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
#include "Textures.h"

int main(int argc, char* argv[]) {																// Add formal parameter list
	int16_t prevState = 0;																		// Previous key state
	bool commsReady = false;
	int i;
    char *srvIPAddr = NULL;																		// Server IP Address string entered from client menu select option								
		
	JOR_NetInit();
    SDL_Init(SDL_INIT_VIDEO);																	// Initialise video only
    TTF_Init();																					// Initialise True Type Fonts
    TTF_Font *font = TTF_OpenFont("Resources/m5x7.ttf", 24);									// Font used to print text to screen

	SDL_Window *window = SDL_CreateWindow( "Cross-Platform SDL Network UDP Game",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);		// JOR Changed game window title, added screen_width & screen_height
    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);								// Game Renderer
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("Could not create renderer: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

	SDL_Texture *imgMap = get_map_texture(renderer);											// Create the map texture (additional tiles added)
	SDL_Texture *imgPlayer1 = load(renderer, "Resources/player1.bmp");							// Sprite for connected clients
	SDL_Texture *imgPlayer2 = load(renderer, "Resources/player2.bmp");							// Additional Player colour for local player
	SDL_Texture *imgBullet = load(renderer, "Resources/bullet1.bmp");

	char menu = 's';																			// Default game type is server
    selectServerOrClient(renderer, &menu, font);												// Set the game as the Client or Server

    if (menu == 'c') {																			// If Client option is selected
        srvIPAddr = malloc(16 * sizeof(char));													// Allocate memory block for Server IP Address
        enterServerIP(renderer, font, srvIPAddr);												// Get the server IP address from the player
    }

	SDL_Thread* threadServerInput = NULL;														// JOR SDL threads replacing pthread, Server input loop
	SDL_Thread* threadServerOutput = NULL;														// Server output loop on separate thread

	struct sockaddr_in srvAddr, srvAddrListen;													// Server address structure, additional listening socket so server can listen on any address (can't send this way in windows)
	JOR_NetInitWinsock();																		// JOR_Net: Initialise winsock
	srvAddr = JOR_NetServAddr(srvIPAddr);														// Init server address structure
	srvAddrListen = JOR_NetServAddr(INADDR_ANY);												// Null for server, IP entered for client
	JOR_NetCliAddr();																			// Init client address structure

    if (menu == 's') {																			// If Server menu option is selected
		commsReady = JOR_NetInitServerUDP(&srvAddr);											// Create Server UDP socket (only one instance of the game is a server)
		commsReady = JOR_NetInitServerListenUDP(&srvAddrListen);								// Initialise the listening socket

		threadServerInput = SDL_CreateThread(serverInputLoop, "ServerReceiveThread", NULL);		// JOR SDL Thread replaces Pthread
		threadServerOutput = SDL_CreateThread(serverOutputLoop, "ServerSendThread", NULL);		// Server output handled on separate thread
    }

	commsReady = JOR_NetInitClientUDP();														// Create client UDP socket (all instances of the game are clients)
	SDL_Thread* threadClient = SDL_CreateThread(clientLoop, "ClientThread", NULL);				// JOR SDL Thread replaces Pthread, Client thread

	while (getClientID() < 0 && commsReady) {													// If the current client is new
		cliSendTo((menu == 's') ? srvAddrListen : srvAddr, getClientID(), 0);					// Set the client ID
		//cliSendTo(srvAddrListen, getClientID(), 0);											// Set the client ID

		JOR_NetSleep(100);																		// Sleep for 100 microseconds
    }

	SDL_Rect bullet_pos = { 0, 0, BULLET_HEIGHT, BULLET_HEIGHT };								// Init bullet object position and dimensions
	SDL_Event e;																				// Handle events
	unsigned int time = 0;
	//unsigned int count = 0;
	//bool keyPressed = false;

    while (1) {
		Player *players = getPlayers();															// Player list

        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) { break; }													// Exit the while loop and close the game
			//keyPressed = resolve_keyboard(e, &players[getClientID()]);							// Handle keyboard input
			resolve_keyboard(e, &players[getClientID()]);							// Handle keyboard input
        }

		if (SDL_GetTicks() > time) {
			//printf("Count: %d\n", count++);

			int16_t keyState = key_state_from_player(&players[getClientID()]);

			//if (keyPressed)
			//if (keyState != 0)
			if (keyState != prevState) {
				prevState = keyState;
				cliSendTo((menu == 's') ? srvAddrListen : srvAddr, getClientID(),					// Only use additional server listening address for server locally
					keyState);								// Send keyboard input data to server
			//cliSendTo((menu == 's') ? srvAddrListen : srvAddr, getClientID(),					// Only use additional server listening address for server locally
			}
			//	key_state_from_player(&players[getClientID()]));								// Send keyboard input data to server

			time = SDL_GetTicks() + 20;															// Update keyboard input every 1/50th of a second
		}
		//cliSendTo(srvAddrListen, getClientID(),	key_state_from_player(&players[getClientID()]));// Send keyboard input data to server
		//JOR_NetSleep(3000);																	// Sleep for 30 microseconds

        SDL_RenderClear(renderer);																// Clear the screen
        SDL_RenderCopy(renderer, imgMap, NULL, NULL);											// Draw the map
		
        for (i = 0; i <= getNumPlayers(); i++) {												// For every player
			renderFlip(renderer, (i == getClientID()) ? 
				imgPlayer2 : imgPlayer1, &players[i].position, players[i].flip);				// Red sprite for local player, blue for connected players
        }

		renderHUD(renderer, font, players, getNumPlayers(), getClientID(), menu);				// Render game info text

        for (i = 0; i < getScreenBullets(); i++) {												// Handle data for bullets on screen
            bullet_pos.x = arrBullets[i*2];														// Every even number is an x coordinate
            bullet_pos.y = arrBullets[i*2 + 1];													// Every odd number is a y coordinate
            SDL_RenderCopy(renderer, imgBullet, NULL, &bullet_pos);								// Draw the bulletes to screen
        }

        SDL_RenderPresent(renderer);
    } // End while

	SDL_WaitThread(threadServerInput, NULL);													// JOR SDL Thread replaces pthread
	SDL_WaitThread(threadClient, NULL);															// Make sure thread finishes before application closes
	SDL_WaitThread(threadServerOutput, NULL);
	JOR_NetCloseSocket();																		// Close the sockets
    SDL_DestroyTexture(imgPlayer1);
    SDL_DestroyTexture(imgBullet);
    SDL_DestroyTexture(imgMap);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
