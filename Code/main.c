#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_thread.h>					// JOR SDL Thread in place of pthread
#include <time.h>
#include <stdint.h>
#include "objects.h"
#include "client_udp.h"
#include "server_udp.h"
#include "network.h"
#include "physic.h"
#include "Definitions.h"
#include "font.h"
#include "menu.h"
#include "Time.h"						// Handle system time on different platforms
#include "SDLFunctions.h"

struct Player players[MAX_PLAYERS];
int numPlayers = 0;						// Number of players currently in the game
int16_t clientID = -1;					// Distinguish between clients, default ID value is -1, if client has another value, they are an existing client
int16_t arrBullets[256];				// Array of bullet objects
int totalBulletsOnScreen = 0;			// Current bullet

SDL_Texture* load_texture(SDL_Renderer *renderer, char *file) {
	SDL_Surface *bitmap = SDL_LoadBMP(file);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, bitmap);
    SDL_FreeSurface(bitmap);
    return texture;
}

void initPlayerList() {
    int i;
    for (i = 0; i < MAX_PLAYERS; i++) {
		players[i].position = makeRect(SPAWN_X, SPAWN_Y, PLAYER_WIDTH, PLAYER_HEIGHT);							// Init player position SDL_Rect
        players[i].left_key = SDLK_LEFT;
        players[i].right_key = SDLK_RIGHT;
        players[i].up_key = SDLK_UP;
        players[i].down_key = SDLK_DOWN;
		players[i].attack_key = SDLK_SPACE;																		// Change fire to spacebar (Was SDLK_z)
        players[i].face = 1;
        players[i].shoot = false;
        players[i].y_speed = 0;
        players[i].can_jump = false;
        players[i].reloading = false;
        players[i].kills = 0;
        players[i].deaths = 0;
		players[i].flip = 0;
	}
}

void setClientID(int id) {
    clientID = id;																								// Set the client id
    numPlayers = id;																							// Number of players in the game
	printf("main.c->setClientID: Client ID is now: %d\n", clientID);
}

void checkIfNewPlayer(int id){
    if (id > numPlayers) {																						// If the client id is higher than the number of players
        numPlayers = id;																						// Set the number of players to match the ID
		printf("Total number of players is now: %d\n", numPlayers + 1);											// The number of players in the game has increased
    }
}

int client_loop(void *arg) {
    int socket = *((int *) arg);																				// cliSock passed in as argument
    int16_t arrData[BUF_MAX];																					// Data to receive from server
	int numBytes, id, bulletsInArray;																			// Number of bytes received , clientID, Active bullets
	bool idSet = false;																							// The client ID has not been set yet

    while (1) {
        numBytes = cliRecvfrom(socket, arrData);																// Receive data from server

        id = arrData[0];																						// Parse received data, first int = id
        if (id == -1 && !idSet) {																				// Parse data when the ID is unset
            setClientID(arrData[1]);																			// Assign an id for the connected player
			idSet = true;																						// Stops the ID being set more than once
        }

        if (id >= 0) {																							// Parse existing Client data
            checkIfNewPlayer(id);																				// Increase number of players if new player added
            players[id].position.x = arrData[1];																// Player x position
            players[id].position.y = arrData[2];																// Player y position
            players[id].kills = arrData[3];																		// Number of kills
            players[id].deaths = arrData[4];																	// Number of times died
			players[id].flip = arrData[5];																		// Client flip (sprite direction)
        }

        if (id == -2) {																							// Parse Bullet data
            bulletsInArray = (numBytes - sizeof(int16_t)) / (sizeof(int16_t) * 2);								// Number of bullets in bullet list
            memcpy(arrBullets, arrData + 1, sizeof(int16_t) * 2 * bulletsInArray);
			if (totalBulletsOnScreen != bulletsInArray) printf("Bullet Total: %d\n", totalBulletsOnScreen);		// Display a count of the number of bullets on screen
            totalBulletsOnScreen = bulletsInArray;
        }

		sleepCrossPlatform(50);																					// Sleep for 50 microseconds
    }

	return 0;																									// Changed function return type to remove incompatible pointer type warning
}

int main(int argc, char* argv[]) {																				// Add formal parameter list
    struct sockaddr_in srvAddr, cliAddr;																		// Server and client address structures
    int i, srvSock, cliSock;																					// for loop index, Server and client sockets
    char *srvIPAddr = NULL;																						// Server IP Address string entered from client menu select option								

	initWinsock();																								// Initialise winsock
    SDL_Init(SDL_INIT_VIDEO);																					// Initialise video only
    TTF_Init();																									// Initialise True Type Fonts
    TTF_Font *font = TTF_OpenFont("../resources/m5x7.ttf", 24);													// Font used to print text to screen
    initPlayerList();																							// Initialise the list of players

	SDL_Window *window = SDL_CreateWindow( "Cross-Platform SDL Network UDP Game",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);						// JOR Changed game window title, added screen_width & screen_height
    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);	// Game Renderer
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("Could not create renderer: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

	SDL_Texture *imgMap = get_map_texture(renderer);															// Create the map texture (additional tiles added)
	SDL_Texture *imgPlayer1 = load_texture(renderer, "../resources/player1.bmp");								// Sprite for connected clients
	SDL_Texture *imgPlayer2 = load_texture(renderer, "../resources/player2.bmp");								// Additional Player colour for local player
	SDL_Texture *imgBullet = load_texture(renderer, "../resources/bullet1.bmp");

	char menu = 's';																							// Default game type is server
    selectServerOrClient(renderer, &menu, font);																// Set the game as the Client or Server

    if (menu == 'c') {																							// If Client option is selected
        srvIPAddr = malloc(16 * sizeof(char));																	// Allocate memory block for Server IP Address
        enterServerIP(renderer, font, srvIPAddr);																// Get the server IP address from the player
    }

	SDL_Thread* threadServerInput = NULL;																		// JOR SDL threads replacing pthread, Server input loop
	SDL_Thread* threadServerOutput = NULL;																		// Server output loop on separate thread

    srvAddr = intServerAddr(srvIPAddr);																			// Init server address structure
    cliAddr = initClientAddr();																					// Init client address structure

    if(menu == 's') {																							// If Server menu option is selected
        createUDPServer(&srvSock, &srvAddr);																	// Create Server UDP socket (only one instance of the game is a server)
		threadServerInput = SDL_CreateThread(serverInputLoop, "ServerReceiveThread", &srvSock);					// JOR SDL Thread replaces Pthread
		threadServerOutput = SDL_CreateThread(serverOutputLoop, "ServerSendThread", &srvSock);					// Server output handled on separate thread
    }
    createClientUDPSock(&cliSock, &cliAddr);																	// Create client UDP socket (all instances of the game are clients)
	SDL_Thread* threadClient = SDL_CreateThread(client_loop, "ClientThread", &cliSock);							// JOR SDL Thread replaces Pthread, Client thread

    while (clientID < 0) {																						// If the current client is new
        cliSendTo(cliSock, srvAddr, clientID, 0);																// Set the client ID
		sleepCrossPlatform(100);																				// Sleep for 100 microseconds
    }

	SDL_Rect bullet_pos = { 0, 0, BULLET_HEIGHT, BULLET_HEIGHT };												// Init bullet object position and dimensions
	SDL_Event e;																								// Handle events

    while (1) {
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) { break; }																	// Exit the while loop and close the game
            resolve_keyboard(e, &players[clientID]);															// Handle keyboard input
        }

        cliSendTo(cliSock, srvAddr, clientID, key_state_from_player(&players[clientID]));						// Send keyboard input data to server

		sleepCrossPlatform(30);																					// Sleep for 30 microseconds

        SDL_RenderClear(renderer);																				// Clear the screen
        SDL_RenderCopy(renderer, imgMap, NULL, NULL);															// Draw the map
		
        for (i = 0; i <= numPlayers; i++) {																		// For every player
			if (i == clientID)																					// If it is the local player
				SDL_RenderCopyEx(renderer, imgPlayer2, NULL, &players[i].position, 0, NULL, players[i].flip);	// Render red sprite for player local player
			else
				SDL_RenderCopyEx(renderer, imgPlayer1, NULL, &players[i].position, 0, NULL, players[i].flip);	// Render blue sprite for connected players
        }

        displayTextWhite(renderer, "Kills:", font, 400, 10);													// HUD: players kills
        for (i = 0; i <= numPlayers; i++) {
            char kills[10] = "";
            snprintf(kills, 3, "%d", players[i].kills);
			if (i == clientID) displayTextRed(renderer, kills, font, 400, 30 + i * 20);							// Display the kills for the local player in red on new line
			else displayTextWhite(renderer, kills, font, 400, 30 + i * 20);										// Display connected players in white
        }

        displayTextWhite(renderer, "Deaths:", font, 460, 10);													// HUD: player deaths
        for (i = 0; i <= numPlayers; i++) {
            char deaths[10] = "";
            snprintf(deaths, 3, "%d", players[i].deaths);
			if (i == clientID) displayTextRed(renderer, deaths, font, 460, 30 + i * 20);						// Display deaths for local player in read beside kills
			else displayTextWhite(renderer, deaths, font, 460, 30 + i * 20);									// Display connected players in white
        }

        for (i = 0; i < totalBulletsOnScreen; i++) {															// Handle data for bullets on screen
            bullet_pos.x = arrBullets[i*2];																		// Every even number is an x coordinate
            bullet_pos.y = arrBullets[i*2 + 1];																	// Every odd number is a y coordinate
            SDL_RenderCopy(renderer, imgBullet, NULL, &bullet_pos);												// Draw the bulletes to screen
        }

        SDL_RenderPresent(renderer);
    } // End while

#if defined __linux__
	close(cliSock);
	close(srvSock);
#elif defined _WIN32 || defined _WIN64
	closesocket(cliSock);																						// Close the client socket
	closesocket(srvSock);																						// Close the server socket
	WSACleanup();																								// Terminate use of Winsock 2 DLL
#endif
	SDL_WaitThread(threadServerInput, NULL);																	// JOR SDL Thread replaces pthread
	SDL_WaitThread(threadClient, NULL);																			// Make sure thread finishes before application closes
	SDL_WaitThread(threadServerOutput, NULL);
    SDL_DestroyTexture(imgPlayer1);
    SDL_DestroyTexture(imgBullet);
    SDL_DestroyTexture(imgMap);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
