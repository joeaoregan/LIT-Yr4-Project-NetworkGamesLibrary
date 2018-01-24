/*
	Joe O'Regan
	K00203642

	Game.cpp

	08/12/2017

	20180117	Game client sends information to UDP Server about player position, and when Lasers are fired
			Altered to only send positional information when the player has changed positions
	20180118	Send object name, and coordinates to server for player, and parse into variables at server side
	20180120	Added a game object list for handling the game objects
			Started implementing a finite state machine
			Added singleton to Texture class
			Moved loading of textures to Texture class
			Moved object rendering to individual classes from Game.cpp
	20180121	Added Audio class to handle game music and effects
			Added finite state machine, and state for playing game
*/


#include "Game.h"							// Game functions

#include <stdio.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h>
#include <string.h>
#include <stdlib.h>
//#include <unistd.h>
//#include "Networking/Socket.h"
//#include "Laser.h"
#include "Input.h"							// 20180120
//#include "Audio.h"							// 20180121

#include "State/MainMenuState.h"
#include "State/PlayState.h"

//#include "Networking/NetJOR.h"


Game* Game::s_pInstance = 0;						// Game singleton

int sock;

bool Game::init() {
	printf("init() called\n");
	
	quit = false;													// Main loop flag	

	gWindow = NULL;
	// Set the players previous position
	// Only update server when player position changes
	//prevX = -1;
	//prevY = -1;

	bool success = true;												// Initialisation flag	

	//createUDPSocket("localhost", "socket test" );									// Moved to NetJOR
	success = NetJOR::Instance()->init();										// Initialise networking stuff
	if (!success) std::cout << "Failed to init() NetJOR" << std::endl;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	} else {
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
			printf( "Warning: Linear texture filtering not enabled!" );
		}
		
		gWindow = SDL_CreateWindow( "Joe O'Regan K00203642 - Year 4 Project", SDL_WINDOWPOS_UNDEFINED, 
					SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );	// Create window
		if( gWindow == NULL ) {
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		} else {
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if( Game::Instance()->getRenderer() == NULL ) {
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else {				
				SDL_SetRenderDrawColor( Game::Instance()->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );	// Initialize renderer color
				
				int imgFlags = IMG_INIT_PNG;								// Initialize PNG loading
				if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
					printf( "SDL_image could not init Error: %s\n", IMG_GetError() );
					success = false;
				}
				 
				if( TTF_Init() == -1 ) {								// Initialize SDL_ttf
					printf( "SDL_ttf could not init Error: %s\n", TTF_GetError() );
					success = false;
				}
				 
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {				// Initialize SDL_mixer
					printf( "SDL_mixer could not init Error: %s\n", Mix_GetError() );
					success = false;
				}
			}
		}
	}

	gFont = TTF_OpenFont( "Assets/Fonts/lazy.ttf", 28 );

	m_pGameStateMachine = NULL;					// init state machine
	m_pGameStateMachine = new GameStateMachine();
	m_pGameStateMachine->changeState(new MainMenuState());
	//m_pGameStateMachine->changeState(new PlayState());

	//printf("init() exit\n");

	return success;
}

void Game::close() {
	printf("close() called\n");

	//sendToServer("3 exit");												// Let the server know to exit ***MOVED TO NetJOR::close()

	//Destroy window	
	SDL_DestroyRenderer( Game::Instance()->getRenderer() );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();

	//closeSocketStuff();													// Moved to NetJOR
	NetJOR::Instance()->close();												// Close networking stuff
}

void Game::update() {
/*
	for (int index = 0; index != listOfGameObjects.size(); ++index) {	
		listOfGameObjects[index]->update();										// Move/Update the game objects
	}

	updateText.str("");													// 20180117			
	updateText << "0 Player1 " << player->getX() << " " << player->getY();							// 20180118 Send name/ID, x coord, y coord  -  to server

	if (player->getX() != prevX || player->getY() != prevY) {								// Only send update if position changes
		sendToServer2(updateText.str().c_str());
		prevX = player->getX();
		prevY = player->getY();
	}
*/

	m_pGameStateMachine->update();												// Update the current state
	NetJOR::Instance()->update();												// Update networking stuff	
}

void Game::render() {	
	SDL_SetRenderDrawColor( Game::Instance()->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );					// Set clear colour
	SDL_RenderClear( Game::Instance()->getRenderer() );									// Clear screen

	m_pGameStateMachine->render();												// Render the current state

	SDL_RenderPresent( Game::Instance()->getRenderer() );									// Update screen
}

void Game::handleEvents() {
	//if (!enterTextState)		// If not in the state for entering text update
	Input::Instance()->update();
	m_pGameStateMachine->handleInput();												// Update the current state
}
/*
void Game::netDestroyGameObject() {
	sendToServer("4 Laser_Destroyed");
}
*/

