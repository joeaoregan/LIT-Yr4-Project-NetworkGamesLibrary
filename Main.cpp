/*
	Joe O'Regan
	K00203642

	Main.cpp

	This is the main entry point for the game
	Includes Game Loop
*/

	
// Game
#include "Game.h"
#include <iostream>

#define SERVER_IP "127.0.0.1"

 int main (int argc, char* argv [])  {

// GAME
	srand(static_cast<unsigned int>(time(0)));			// Seed the random number


	const char* server = (argc == 1) ? SERVER_IP : argv[1];
	//Game::Instance()->init( server );				// Initialise the game SOCKET, use localhost if no IP address specified

	Game::Instance()->init();					// Initialise the game
	//Game::Instance()->createUDPSocket(argv[1]);	// Create a UDP socket

	if (!Game::Instance()->loadMedia())				// Load the game
		printf( "Failed to load media!\n" );
	else {
		while (!Game::Instance()->gameFinished()) {
			Game::Instance()->update();			// Game singleton used to call update()
		}
	}

	Game::Instance()->close();

	//system("pause");						// Windows

	return 0;
}


