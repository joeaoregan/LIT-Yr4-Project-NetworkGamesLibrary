#ifndef HUD_H
#define HUD_H

#include <SDL.h>			// renderer
#include <SDL_ttf.h>		// font
#include "objects.h"		// struct Player players

void renderHUD(SDL_Renderer *renderer, TTF_Font *font, struct Player *players, int numPlayers, int clientID, char menu);

#endif