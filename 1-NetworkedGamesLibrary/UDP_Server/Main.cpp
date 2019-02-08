/*
	Joe O'Regan
	K00203642

	Main.cpp
	30/01/2018

	contains game server loop
*/
#include "GameServer.h"

GameServer gs;

int main() {
	gs.init();

	//while (gs.getNumPlayers() <= gs.getMaxNetPlayers()){	// While the number of players is less than the maximum number of players
		gs.addPlayers();				// Add players to the game server
	//}
	
	while (1) {
		gs.update();					// Send and receive game data
	}

	gs.close_gs();
}
