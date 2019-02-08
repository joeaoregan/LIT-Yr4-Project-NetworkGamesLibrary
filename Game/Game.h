/*
	Joe O'Regan
	K00203642

	Game.h

	Cross-platform main game header file
	Using singleton pattern to access variables and functionality
*/

#ifndef	__GAME_H
#define	__GAME_H

// Unix (Ubuntu)
#if defined __linux__
#include <SDL2/SDL_mixer.h>							// SDL Audio
#include <SDL2/SDL_ttf.h>							// True type fonts
#include <SDL2/SDL_image.h>							// Images (PNG etc.)
// Windows
#elif defined _WIN32 || defined _WIN64
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#endif

#include <cstdlib>								// For Random Numbers
#include <ctime>								// For Random Numbers
#include <string>
#include "State/GameStateMachine.h"

#include "../Networking/NetJOR.h"

#define SCREEN_WIDTH 1280							// Screen Width
#define SCREEN_HEIGHT 720							// Screen Height


# define TCP_PORT 1066

class Game {
public:	
	// Game Singleton
	static Game* Instance() {
		if (s_pInstance == 0) {						// If there is no instance of Game
			s_pInstance = new Game();				// Create new game instance
			return s_pInstance;					// and return it
		}

		return s_pInstance;						// Return the current game instance
	}

	bool init();								// Starts up SDL and creates window
	void update();								// Update the game
	void render();								// Draw the game to screen
	void close();								// Frees media and shuts down SDL
	void handleEvents();

	SDL_Renderer* getRenderer() const { return gRenderer; }			// Get the renderer

	TTF_Font* getFont() {return gFont;}
	
	bool gameFinished() { return quit; }					// Has the game exited
	void setGameFinished() { quit = true; }					// Has the game exited
		
	//void netDestroyGameObject();

	SDL_Event getEvent() { return event;}

	GameStateMachine* getStateMachine() { return m_pGameStateMachine; }	// Get the game state machine

	int getAssignedNetID() { return m_choosePlayer; }			// The network assigns a number to each player on connecting
	void setAssignedNetID(int set) { m_choosePlayer = set; }		// Store the ID assigned by the server

private:
	static Game* s_pInstance;						// Game singleton instance

	SDL_Renderer* gRenderer = NULL;						// The window renderer
	SDL_Window* gWindow;							// The window we'll be rendering to

	GameStateMachine* m_pGameStateMachine;

	TTF_Font *gFont = NULL;							// Globally used font
	
	bool quit;								// Main game loop condition
	
	SDL_Event event;							// Event handler (Changed - was pointer)
	
	int m_choosePlayer;							// Network stuff
};

#endif	/* __GAME_H */
