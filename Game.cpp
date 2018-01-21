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
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "Socket.h"
#include "Game.h"							// Game functions
#include "Texture.h"							// Texture functions
#include "Player.h"							// Player functions
#include "Laser.h"
#include <list>
#include <sstream>							// 20180117 Updating text
#include "Input.h"							// 20180120
#include "Background.h"							// 20180121
#include "Audio.h"							// 20180121

std::stringstream updateText;

Game* Game::s_pInstance = 0;						// Game singleton
std::vector<GameObject*> listOfGameObjects;				// List of game objects

GameObject* background;	 						// Scrolling background
GameObject* player;							// The Player that will be moving around on the screen	

// UI textures
Texture gTextTexture;

//Mix_Chunk *laserFX = NULL;

int sock;

bool Game::init() {	
	printf("init() called\n");
	// Create player and background, and add to game object list
	background = new Background();
	player = new Player();
	listOfGameObjects.push_back(background);
	listOfGameObjects.push_back(player);

	player->spawn(0,(SCREEN_HEIGHT - player->getHeight() - 120)/2, player->getVel());				// Center of play area
	
	quit = false;													// Main loop flag	

	gWindow = NULL;
	// Set the players previous position
	// Only update server when player position changes
	prevX = -1;
	prevY = -1;

	createUDPSocket("localhost", "socket test" );

	bool success = true;												// Initialization flag

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

	//printf("init() exit\n");

	return success;
}

bool Game::loadMedia() {	
	printf("loadMedia() called\n");
	bool success = true;	

	success = Texture::Instance()->loadTextures();									// Load the game textures
/*															// Loading success flag
	laserFX = Mix_LoadWAV( "Assets/Effects/scratch.wav" );
	if( laserFX == NULL ) {
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
*/
	//Open the font
	gFont = TTF_OpenFont( "Assets/Fonts/lazy.ttf", 28 );
	if( gFont == NULL ) {
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	} else {
		//Render text
		SDL_Color textColor = { 255, 0, 0 };
		if( !gTextTexture.loadFromRenderedText( "Linux SDL Test Game - Joe O'Regan", textColor )) {
			printf( "Failed to render text texture!\n" );
			success = false;
		}
	}

	return success;
}

void Game::close() {
	printf("close() called\n");
	//Free loaded images
	gTextTexture.free();

	//Free the sound effects
	//Mix_FreeChunk( laserFX );
	//laserFX = NULL;

	//Destroy window	
	SDL_DestroyRenderer( Game::Instance()->getRenderer() );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();

	closeSocketStuff();
}

void Game::update() {	
	while( SDL_PollEvent( &e ) != 0 ) {											// Handle events on queue				
		if( e.type == SDL_QUIT ) {											// User requests quit
			quit = true;

			sendToServer("3 exit");											// Let the server know to exit
		}
		
		player->handleEvent( e );											// Handle input for the Player
	}															// Move the Player

	for (int index = 0; index != listOfGameObjects.size(); ++index) {	
		listOfGameObjects[index]->move();										// Move the game object
	}

	updateText.str("");													// 20180117			
	updateText << "0 Player1 " << player->getX() << " " << player->getY();							// 20180118 Send name/ID, x coord, y coord  -  to server

	if (player->getX() != prevX || player->getY() != prevY) {								// Only send update if position changes
		sendToServer2(updateText.str().c_str());
		prevX = player->getX();
		prevY = player->getY();
	}
}

void Game::render() {	
	SDL_SetRenderDrawColor( Game::Instance()->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );					// Set clear colour
	SDL_RenderClear( Game::Instance()->getRenderer() );									// Clear screen

	for (int index = 0; index != listOfGameObjects.size(); ++index) {	
		listOfGameObjects[index]->render();										// Render the game object
	}

	gTextTexture.render((SCREEN_WIDTH - gTextTexture.getWidth()) / 2,((SCREEN_HEIGHT -600 -gTextTexture.getHeight())/ 2)+ 600);

	SDL_RenderPresent( Game::Instance()->getRenderer() );									// Update screen
}

void Game::handleEvents() {
	Input::Instance()->update();
}

void Game::spawnLaser() {
	std::cout << "Laser Spawned" << std::endl;
//	Mix_PlayChannel( -1, laserFX, 0 );											// 20180120 Sound effects not playing now ???
//	std::cout << "sound" << std::endl;
	Audio::Instance()->playFX("laserFX");
	GameObject* p_Laser1 = new Laser();											// 20180120 Add laser to game object list 
	listOfGameObjects.push_back(p_Laser1);
	p_Laser1->spawn(player->getX() + 65, player->getY() + 30, 20);
	sendToServer("1 Player_Laser_Fired");											// Notify server when laser fired
}

void Game::netDestroyGameObject() {
	sendToServer("4 Laser_Destroyed");
}

