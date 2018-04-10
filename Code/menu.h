/*
	Modified by:	Joe O'Regan
					K00203642

	menu.h

	Added JOR_Net library for communication
	Added additional text, and keyboard keypad input
*/

#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "Text.h"																// Display game text
#include "Definitions.h"

void selectServerOrClient(SDL_Renderer *renderer, char *menu, TTF_Font *font);
void enterServerIP(SDL_Renderer *renderer, TTF_Font *font, char *ip);

#endif
