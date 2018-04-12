/*
	Created by: Joe O'Regan
				K00203642

	HUD.c

	Moved HUD info text here
*/


#include "HUD.h"
#include "Text.h"	// Display game text
#include <stdio.h>	// snprintf()

int highlightLocalPlayerText(SDL_Renderer *renderer, TTF_Font *font, int posX, int data, int clientID, int index) {
	char label[10] = "";
	int numbytes = snprintf(label, 3, "%d", data);
	if (index == clientID) displayTextRed(renderer, label, font, posX, 30 + index * 20);							/// Display the kills for the local player in red on new line
	else displayTextWhite(renderer, label, font, posX, 30 + index * 20);											// Display connected players in white

	return numbytes;
}

void renderHUD(SDL_Renderer *renderer, TTF_Font *font, Player *players, int numPlayers, int clientID, char menu) {
	int i, numbytes;

	if (menu == 's') 
		displayTextRed(renderer, "SERVER / Client 0", font, 10, 10);												// Indicate the game is the Server
	else {
		char cliText[12] = "";
		numbytes = snprintf(cliText, 10, "Client: %d", clientID);													// Use instead of snprintf_s for Linux
		displayTextWhite(renderer, cliText, font, 10, 10);															// HUD: Client Number
	}

	displayTextWhite(renderer, "Kills:", font, 400, 10);															// HUD: players kills
	for (i = 0; i <= numPlayers; i++) {
		numbytes = highlightLocalPlayerText(renderer, font, 400, players[i].kills, clientID, i);					// Display the kills for the local player in red on new line, connected players in white
	}

	displayTextWhite(renderer, "Deaths:", font, 460, 10);															// HUD: player deaths
	for (i = 0; i <= numPlayers; i++) {
		numbytes = highlightLocalPlayerText(renderer, font, 460, players[i].deaths, clientID, i);					// Display the deaths for the local player in red on new line, connected players in white
	}

	if (numbytes == 0) printf("Error initialising HUD\n");
}

