/*
	Modified by:	Joe O'Regan
					K00203642

	Originally by:	Maciej Spychala

	menu.h

	Added JOR_Net library for communication
	Added additional text, and keyboard keypad input
	Added default of localhost when enter is pressed
*/

#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "Definitions.h"

void selectServerOrClient(SDL_Renderer *renderer, char *menu, TTF_Font *font);	// Display menu option to select Server or Client
void enterServerIP(SDL_Renderer *renderer, TTF_Font *font, char *ip);			// Enter the IP address of the server

#endif
