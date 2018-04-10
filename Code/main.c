/*
	Modified by:	Joe O'Regan
					K00203642

	main.c

	Added JOR_Net library for communication
*/

#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_thread.h>					// JOR SDL Thread in place of pthread
#include <time.h>
#include <stdint.h>
#include "GameObject.h"					// Game objects Player and Bullet
#include "ClientThread.h"				// Thread to handle client side communication
#include "ServerThreads.h"				// Threads to handle server input and output
#include "Input.h"						// Handle player keyboard input
#include "physic.h"
#include "Definitions.h"
#include "Text.h"						// Display game text
#include "menu.h"
#include "SDLFunctions.h"
#include "HUD.h"						// Heads up display
#include "JOR_Net.h"					// UDP Network game library

SDL_Texture* load_texture(SDL_Renderer *renderer, char *file) {
	SDL_Surface *bitmap = SDL_LoadBMP(file);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, bitmap);
    SDL_FreeSurface(bitmap);
    return texture;
}

/*
int client_loop(void *arg) {
    //int socket = *((int *) arg);																				// cliSock passed in as argument
    int16_t arrData[JN_BUF_MAX];																				// Data to receive from server
	int numBytes, id, bulletsInArray;																			// Number of bytes received , clientID, Active bullets
	bool idSet = false;																							// The client ID has not been set yet

    while (1) {
		//numBytes = cliRecvfrom(socket, arrData);																// Receive data from server
		numBytes = cliRecvfrom(arrData);																		// Receive data from server

        id = arrData[0];																						// Parse received data, first int = id
        if (id == -1 && !idSet) {																				// Parse data when the ID is unset
			JOR_NetSetClientID(arrData[1], &clientID, &numPlayers);												// xxx Assign an id for the connected player
			idSet = true;																						// Stops the ID being set more than once
        }

        if (id >= 0) {																							// Parse existing Client data
			JOR_NetCheckNewClient(id, &numPlayers);																// Increase number of players if new player added
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

		JOR_NetSleep(50);																						// Sleep for 50 microseconds
    }

	return 0;																									// Changed function return type to remove incompatible pointer type warning
}
*/

int main(int argc, char* argv[]) {																				// Add formal parameter list
	bool commsReady = false;
	int i;
    char *srvIPAddr = NULL;																						// Server IP Address string entered from client menu select option								
		
    SDL_Init(SDL_INIT_VIDEO);																					// Initialise video only
    TTF_Init();																									// Initialise True Type Fonts
    TTF_Font *font = TTF_OpenFont("../resources/m5x7.ttf", 24);													// Font used to print text to screen

	SDL_Window *window = SDL_CreateWindow( "Cross-Platform SDL Network UDP Game",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);						// JOR Changed game window title, added screen_width & screen_height
    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);												// Game Renderer
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

	struct sockaddr_in srvAddr, cliAddr;																		// Server and client address structures
	JOR_NetInitWinsock();																						// JOR_Net: Initialise winsock
	srvAddr = JOR_NetServAddr(srvIPAddr);																		// Init server address structure
    cliAddr = JOR_NetCliAddr();																					// Init client address structure

    if (menu == 's') {																							// If Server menu option is selected
		commsReady = JOR_NetInitServerUDP(&srvAddr);															// Create Server UDP socket (only one instance of the game is a server)
		threadServerInput = SDL_CreateThread(serverInputLoop, "ServerReceiveThread", NULL);						// JOR SDL Thread replaces Pthread
		threadServerOutput = SDL_CreateThread(serverOutputLoop, "ServerSendThread", NULL);						// Server output handled on separate thread
    }

	commsReady = JOR_NetClientUDPSock(&cliAddr);																// Create client UDP socket (all instances of the game are clients)
	SDL_Thread* threadClient = SDL_CreateThread(clientLoop, "ClientThread", NULL);								// JOR SDL Thread replaces Pthread, Client thread

    while (getClientID() < 0 && commsReady) {																	// If the current client is new

		//printf("getClientID()  %d\n", getClientID());
		cliSendTo(srvAddr, getClientID(), 0);																	// Set the client ID

		JOR_NetSleep(100);																						// Sleep for 100 microseconds
    }

	SDL_Rect bullet_pos = { 0, 0, BULLET_HEIGHT, BULLET_HEIGHT };												// Init bullet object position and dimensions
	SDL_Event e;																								// Handle events

    while (1) {
		struct Player *players = getPlayers();

        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) { break; }																	// Exit the while loop and close the game
			resolve_keyboard(e, &players[getClientID()]);														// Handle keyboard input
			//printf("getClientID() Main %d\n", getClientID());
        }

		cliSendTo(srvAddr, getClientID(), key_state_from_player(&players[getClientID()]));						// Send keyboard input data to server

		JOR_NetSleep(30);																						// Sleep for 30 microseconds

        SDL_RenderClear(renderer);																				// Clear the screen
        SDL_RenderCopy(renderer, imgMap, NULL, NULL);															// Draw the map
		
        for (i = 0; i <= getNumPlayers(); i++) {																// For every player
			if (i == getClientID())																				// If it is the local player
				SDL_RenderCopyEx(renderer, imgPlayer2, NULL, &players[i].position, 0, NULL, players[i].flip);	// Render red sprite for player local player
			else
				SDL_RenderCopyEx(renderer, imgPlayer1, NULL, &players[i].position, 0, NULL, players[i].flip);	// Render blue sprite for connected players
        }

		renderHUD(renderer, font, players, getNumPlayers(), getClientID(), menu);								// Render game info text

        for (i = 0; i < getScreenBullets(); i++) {																// Handle data for bullets on screen
            bullet_pos.x = arrBullets[i*2];																		// Every even number is an x coordinate
            bullet_pos.y = arrBullets[i*2 + 1];																	// Every odd number is a y coordinate
            SDL_RenderCopy(renderer, imgBullet, NULL, &bullet_pos);												// Draw the bulletes to screen
        }

        SDL_RenderPresent(renderer);
    } // End while

	JOR_NetCloseSocket();																						// Close the sockets

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

