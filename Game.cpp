/*
	Joe O'Regan
	K00203642
	08/12/2017
*/

#include <stdio.h>
#include "Game.h"							// Game functions
#include "Texture.h"							// Texture functions
#include "Player.h"							// Player functions
#include "Laser.h"
#include <list>

Game* Game::s_pInstance = 0;						// Game singleton

SDL_Window* gWindow = NULL;						// The window we'll be rendering to

// Scene textures
Texture gPlayerTexture;
Texture gBGTexture;
Texture gLaserTexture;
Texture gTextTexture;

Mix_Chunk *laserFX = NULL;

Player player;								// The Player that will be moving around on the screen	


std::list<Laser*> listOfLaserObjects;					// List to store laser objects
std::list<Laser*>::const_iterator iter;					// Make them read only

void Player::render() {    
	gPlayerTexture.render( player.getX(), player.getY() );		// Show the Player
}

bool Game::init() {	
	bool success = true;						// Initialization flag

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	} else {
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
			printf( "Warning: Linear texture filtering not enabled!" );
		}
		
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );	// Create window
		if( gWindow == NULL ) {
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		} else {
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( Game::Instance()->getRenderer() == NULL ) {
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else {				
				SDL_SetRenderDrawColor( Game::Instance()->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );						// Initialize renderer color
				
				int imgFlags = IMG_INIT_PNG;													// Initialize PNG loading
				if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				 
				if( TTF_Init() == -1 ) {													// Initialize SDL_ttf
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
				 
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {									// Initialize SDL_mixer
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					success = false;
				}
			}
		}
	}

	player.spawn(0,(SCREEN_HEIGHT - player.getY())/2, player.getVel());

	return success;
}

bool Game::loadMedia() {	
	bool success = true;												// Loading success flag
	laserFX = Mix_LoadWAV( "Assets/Effects/scratch.wav" );
	if( laserFX == NULL ) {
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	if( !gPlayerTexture.loadFromFile( "Assets/Art/Player1Ship.png" ) ) {						// Load Player texture
		printf( "Failed to load Player texture!\n" );
		success = false;
	}
	
	if( !gBGTexture.loadFromFile( "Assets/Art/bg720.png" ) ) {							// Load background texture
		printf( "Failed to load background texture!\n" );
		success = false;
	}

	if( !gLaserTexture.loadFromFile( "Assets/Art/LaserGreen.png" ) ) {						// Load background texture
		printf( "Failed to load background texture!\n" );
		success = false;
	}

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
	//Free loaded images
	gPlayerTexture.free();
	gBGTexture.free();
	gLaserTexture.free();
	gTextTexture.free();

	//Free the sound effects
	Mix_FreeChunk( laserFX );
	laserFX = NULL;

	//Destroy window	
	SDL_DestroyRenderer( Game::Instance()->getRenderer() );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void Game::update() {
	if( !init() ) {													// Start up SDL and create window
		printf( "Failed to initialize!\n" );
	} else {		
		if( !loadMedia() ) {											// Load game media
			printf( "Failed to load media!\n" );
		} else {				
			bool quit = false;										// Main loop flag			
			SDL_Event e;											// Event handler	
			int scrollingOffset = 0;									// Background scrolling offset
			
			while( !quit ) {										// While application is running			
				while( SDL_PollEvent( &e ) != 0 ) {							// Handle events on queue				
					if( e.type == SDL_QUIT ) {							// User requests quit
						quit = true;
					}
					
					player.handleEvent( e );							// Handle input for the Player
				}
				
				player.move();										// Move the Player
				
				--scrollingOffset;									// Decrement: Moves right to left on X axis
				if( scrollingOffset < -gBGTexture.getWidth() ) {					// Scroll background
					scrollingOffset = 0;
				}
				
				SDL_SetRenderDrawColor( Game::Instance()->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );	// Set clear colour
				SDL_RenderClear( Game::Instance()->getRenderer() );					// Clear screen
				
				gBGTexture.render( scrollingOffset, 0 );						// Render background
				gBGTexture.render( scrollingOffset + gBGTexture.getWidth(), 0 );			// Render background part 2
				
				player.render();									// Render objects

				for (iter = listOfLaserObjects.begin(); iter != listOfLaserObjects.end();) {
					
					(*iter++)->render();								// Render the laser
				}

				gTextTexture.render( ( SCREEN_WIDTH - gTextTexture.getWidth() ) / 2, ( (SCREEN_HEIGHT - 600 - gTextTexture.getHeight() ) / 2 )+ 600);

				SDL_RenderPresent( Game::Instance()->getRenderer() );					// Update screen



				for (iter = listOfLaserObjects.begin(); iter != listOfLaserObjects.end();) {
					(*iter++)->move();								// Move the laser
				}
			}
		}
	}		
}

void Game::spawnLaser() {
	Mix_PlayChannel( -1, laserFX, 0 );
	Laser* p_Laser = new Laser();
	p_Laser->spawn(player.getX() + 65, player.getY() + 30, 20);
	listOfLaserObjects.push_back(p_Laser);
}

void Laser::render() {
	gLaserTexture.render(getX(), getY());										// Show the Laser
}

