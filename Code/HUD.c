/*
	Joe O'Regan
	K00203642

	Moved HUD info text here
*/

#include "HUD.h"
#include "font.h"
#include <stdio.h>	// snprintf()

void renderHUD(SDL_Renderer *renderer, TTF_Font *font, struct Player *players, int numPlayers, int clientID, char menu) {
	int i;

	if (menu == 's') 
		displayTextRed(renderer, "SERVER / Client 0", font, 10, 10);										// Indicate the game is the Server
	else {
		char cliText[12] = "";
		snprintf(cliText, 10, "Client: %d", clientID);
		displayTextWhite(renderer, cliText, font, 10, 10);													// HUD: Client Number
	}

	displayTextWhite(renderer, "Kills:", font, 400, 10);													// HUD: players kills
	for (i = 0; i <= numPlayers; i++) {
		char kills[10] = "";
		snprintf(kills, 3, "%d", players[i].kills);
		if (i == clientID) displayTextRed(renderer, kills, font, 400, 30 + i * 20);							// Display the kills for the local player in red on new line
		else displayTextWhite(renderer, kills, font, 400, 30 + i * 20);										// Display connected players in white
	}

	displayTextWhite(renderer, "Deaths:", font, 460, 10);													// HUD: player deaths
	for (i = 0; i <= numPlayers; i++) {
		char deaths[10] = "";
		snprintf(deaths, 3, "%d", players[i].deaths);
		if (i == clientID) displayTextRed(renderer, deaths, font, 460, 30 + i * 20);						// Display deaths for local player in read beside kills
		else displayTextWhite(renderer, deaths, font, 460, 30 + i * 20);									// Display connected players in white
	}
}