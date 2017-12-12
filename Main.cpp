
/*
	MAIN:
	This is the main entry point for the game
*/

	
// Game
#include "Game.h"
#include <iostream>

 int main (int argc, char * argv [])  {
//int main( ) {	
	


// GAME
	srand(static_cast<unsigned int>(time(0)));		// Seed the random number

//	Game::Instance()->init(argv[1]);			// Initialise the game SOCKET
	Game::Instance()->init();				// Initialise the game

	if (!Game::Instance()->loadMedia())			// Load the game
		printf( "Failed to load media!\n" );
	else {
		while (!Game::Instance()->gameFinished()) {
			Game::Instance()->update();		// Game singleton used to call update()
		}
	}

	//Game::Instance()->update();				// Game singleton used to call update()
	Game::Instance()->close();

	//system("pause");					// Windows

	return 0;
}


