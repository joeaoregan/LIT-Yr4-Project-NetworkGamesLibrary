/*
	Joe O'Regan
	K00203642

	HUD.h

	Moved HUD info text here
*/

#ifndef HUD_H
#define HUD_H

#include <SDL.h>			// renderer
#include <SDL_ttf.h>		// font
#include "GameObject.h"		// Game objects Player and Bullet

void renderHUD(SDL_Renderer *renderer, TTF_Font *font, Player *players, int numPlayers, int clientID, char menu);

#endif
