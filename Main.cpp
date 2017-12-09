
/*
	MAIN:
	This is the main entry point for the game
*/

#include "Game.h"
 #include <iostream>

int main( int argc, char* args[] ) {	
//int main( ) {	
	srand(static_cast<unsigned int>(time(0)));	// Seed the random number


	Game::Instance()->update();			// Game singleton used to call update()
	Game::Instance()->close();

	//system("pause");				// Windows

	return 0;
}
