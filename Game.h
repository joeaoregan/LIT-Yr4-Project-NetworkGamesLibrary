/*
	Joe O'Regan
	K00203642

	Game.h

	Main game file
	Using singleton pattern to access variables and functionality
*/
#ifndef	__GAME_H
#define	__GAME_H

#include <SDL2/SDL.h>						// SDL
#include <SDL2/SDL_mixer.h>					// Audio
#include <SDL2/SDL_ttf.h>					// True type fonts
#include <cstdlib>						// For Random Numbers
#include <ctime>						// For Random Numbers
#include <string>

//#include "Socket.h"

#define SCREEN_WIDTH 1280					// Screen Width
#define SCREEN_HEIGHT 720					// Screen Height


# define TCP_PORT 1066

class Game {
public:	

	// Game Singleton
	static Game* Instance() {
		if (s_pInstance == 0) {				// If there is no instance of Game
			s_pInstance = new Game();		// Create new game instance
			return s_pInstance;			// and return it
		}

		return s_pInstance;				// Return the current game instance
	}

	//bool init(const char* serverName);			// Starts up SDL and creates window
	bool init();						// Starts up SDL and creates window
	//void createUDPSocket(char* serverName);		// Create a UDP Socket

	bool loadMedia();					// Loads media

	void update();						// Update the game

	void close();						// Frees media and shuts down SDL

	SDL_Renderer* getRenderer() const { return gRenderer; }	// Get the renderer

	TTF_Font* getFont() {return gFont;}


	void spawnLaser();
	
	bool gameFinished() { return quit; }			// Has the game exited



private:
	static Game* s_pInstance;				// Game singleton instance

	SDL_Renderer* gRenderer = NULL;				// The window renderer

	TTF_Font *gFont = NULL;					// Globally used font
	
	bool quit;						// Main game loop condition
	
	SDL_Event e;						// Event handler

	int scrollingOffset;

	int prevX, prevY;
};

#endif	/* __GAME_H */
