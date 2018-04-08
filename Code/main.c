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
    //SDL_Surface *bitmap = NULL;
    //SDL_Texture *texture = NULL;
	SDL_Surface *bitmap = SDL_LoadBMP(file);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, bitmap);
    SDL_FreeSurface(bitmap);
    return texture;
}


void initPlayerList() {
    int i;
    for (i = 0; i < MAX_PLAYERS; i++) {
		players[i].position = makeRect(SPAWN_X, SPAWN_Y, PLAYER_WIDTH, PLAYER_HEIGHT);			// Init player position SDL_Rect
        players[i].left_key = SDLK_LEFT;
        players[i].right_key = SDLK_RIGHT;
        players[i].up_key = SDLK_UP;
        players[i].down_key = SDLK_DOWN;
		players[i].attack_key = SDLK_SPACE;														// Change fire to spacebar (Was SDLK_z)
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
    clientID = id;																					// Set the client id
    numPlayers = id;																				// Number of players in the game
	printf("main.c->setClientID: Client ID is now: %d\n", clientID);
}

void checkIfNewPlayer(int id){
    if (id > numPlayers) {																			// If the client id is higher than the number of players
        numPlayers = id;																			// Set the number of players to match the ID
		printf("Total number of players is now: %d\n", numPlayers + 1);								// The number of players in the game has increased
    }
}

int client_loop(void *arg) {
    int socket = *((int *) arg);																	// cliSock passed in as argument
    int16_t arrData[BUF_MAX];																		// Data to receive from server
	int numBytes, id, bulletsInArray;																// Number of bytes received , clientID, Active bullets
	bool idSet = false;																				// The client ID has not been set yet

    while (1) {
        numBytes = cliRecvfrom(socket, arrData);													// Receive data from server

		// Parse received data
        id = arrData[0];																			// Parse received data, first int = id
        if (id == -1 && !idSet) {																	// If the ID is unset
            setClientID(arrData[1]);																// Assign an id for the connected player
			idSet = true;																			// Stops the ID being set more than once
        }

		// Client data from Server
        if (id >= 0) {
            checkIfNewPlayer(id);																	// Increase number of players if new player added
            players[id].position.x = arrData[1];													// Player x position
            players[id].position.y = arrData[2];													// Player y position
            players[id].kills = arrData[3];															// Number of kills
            players[id].deaths = arrData[4];														// Number of times died
			players[id].flip = arrData[5];															// Client flip (sprite direction)
        }

        if (id == -2) {																				// Bullet data
            bulletsInArray = (numBytes - sizeof(int16_t)) / (sizeof(int16_t) * 2);
            memcpy(arrBullets, arrData + 1, sizeof(int16_t) * 2 * bulletsInArray);
			if (totalBulletsOnScreen != bulletsInArray) 
				printf("Bullet Total: %d\n", totalBulletsOnScreen);									// Display a count of the number of bullets on screen
            totalBulletsOnScreen = bulletsInArray;
        }

		sleepCrossPlatform(50);																		// Sleep for 50 microseconds
    }

	return 0;																						// Changed function return type to remove incompatible pointer type warning
}

int main(int argc, char* argv[]) {																	// Add formal parameter list
    struct sockaddr_in srvAddr, cliAddr;															// Server and client address structures
    int srvSock, cliSock;																			// Server and client sockets
    char *srvIPAddr = NULL;																			// Server IP Address entered from client menu select option								

	initWinsock();																					// Initialise winsock

    char menu = 's';																				// Default game type is server
    SDL_Window *window;																				// Game window
    SDL_Renderer *renderer;																			// Game renderer
    SDL_Init(SDL_INIT_VIDEO);																		// Initialise video only
	SDL_Texture *imgPlayer1 = NULL;																	// Sprite for connected clients
	SDL_Texture *imgPlayer2 = NULL;																	// Additional Player colour for local player
    SDL_Texture *imgBullet = NULL;
    SDL_Texture *imgMap = NULL;
    TTF_Init();
    TTF_Font *font;
    font = TTF_OpenFont("../resources/m5x7.ttf", 24);
    initPlayerList();																				// Initialise the list of players
    window = SDL_CreateWindow( "Cross-Platform SDL Network UDP Game", 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);			// JOR Changed window title, added screen_width & screen_height

    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("Could not create renderer: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    imgMap = get_map_texture(renderer);
	imgPlayer1 = load_texture(renderer, "../resources/player1.bmp");
	imgPlayer2 = load_texture(renderer, "../resources/player2.bmp");
    imgBullet = load_texture(renderer, "../resources/bullet1.bmp");

    int i;
    selectServerOrClient(renderer, &menu, font);													// Set the game as the Client or Server

    if (menu == 'c') {																				// If Client option is selected
        srvIPAddr = malloc(16 * sizeof(char));														// Allocate memory block for Server IP Address
        enterServerIP(renderer, font, srvIPAddr);													// Get the server IP address from the player
    }

	SDL_Thread* threadServerInput = NULL;															// JOR SDL threads replacing pthread
	SDL_Thread* threadClient = NULL;							
	SDL_Thread* threadServerOutput = NULL;

    srvAddr = intServerAddr(srvIPAddr);
    cliAddr = initClientAddr();

    if(menu == 's') {
        createUDPServer(&srvSock, &srvAddr);
		threadServerInput = SDL_CreateThread(serverInputLoop, "ServerReceiveThread", &srvSock);		// JOR SDL Thread replaces Pthread
		threadServerOutput = SDL_CreateThread(serverOutputLoop, "ServerSendThread", &srvSock);
    }
    createClientUDPSock(&cliSock, &cliAddr);
	threadClient = SDL_CreateThread(client_loop, "ClientThread", &cliSock);							// JOR SDL Thread replaces Pthread

    while (clientID < 0) {
        cliSendTo(cliSock, srvAddr, clientID, 0);

		sleepCrossPlatform(100);																	// Sleep for 100 microseconds
    }

	SDL_Rect bullet_pos;
	bullet_pos.w = BULLET_HEIGHT;
	bullet_pos.h = BULLET_HEIGHT;

	SDL_Event e;

    while (1) {
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) { break; }														// Exit the while loop and close the game
            resolve_keyboard(e, &players[clientID]);
        }

        cliSendTo(cliSock, srvAddr, clientID, key_state_from_player(&players[clientID]));

		sleepCrossPlatform(30);																		// Sleep for 30 microseconds

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, imgMap, NULL, NULL);
		
        for (i = 0; i <= numPlayers; i++) {
			if (i == clientID)
				SDL_RenderCopyEx(renderer, imgPlayer2, NULL, &players[i].position, 0, NULL, players[i].flip);	// Red for player 2
			else
				SDL_RenderCopyEx(renderer, imgPlayer1, NULL, &players[i].position, 0, NULL, players[i].flip);	// Blue for player 1
        }

        displayText(renderer, "Kills:", font, 400, 10);

        for (i = 0; i <= numPlayers; i++) {
            char kills[10] = "";
            snprintf(kills, 3, "%d", players[i].kills);
            displayText(renderer, kills, font, 400, 30 + i * 20);
        }

        displayText(renderer, "Deaths:", font, 460, 10);

        for (i = 0; i <= numPlayers; i++) {
            char deaths[10] = "";
            snprintf(deaths, 3, "%d", players[i].deaths);
            displayText(renderer, deaths, font, 460, 30 + i * 20);
        }

        for (i = 0; i < totalBulletsOnScreen; i++) {
            bullet_pos.x = arrBullets[i*2];								// Even numbers
            bullet_pos.y = arrBullets[i*2 + 1];							// Odd numbers
            SDL_RenderCopy(renderer, imgBullet, NULL, &bullet_pos);
        }

        SDL_RenderPresent(renderer);
    } // End while

#if defined __linux__
	close(cliSock);
	close(srvSock);
#elif defined _WIN32 || defined _WIN64
	closesocket(cliSock);																			// Close the client socket
	closesocket(srvSock);																			// Close the server socket
	WSACleanup();																					// Terminate use of Winsock 2 DLL
#endif
	SDL_WaitThread(threadServerInput, NULL);														// JOR SDL Thread replaces pthread
	SDL_WaitThread(threadClient, NULL);																// Make sure thread finishes before application closes
	SDL_WaitThread(threadServerOutput, NULL);
    SDL_DestroyTexture(imgPlayer1);
    SDL_DestroyTexture(imgBullet);
    SDL_DestroyTexture(imgMap);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
