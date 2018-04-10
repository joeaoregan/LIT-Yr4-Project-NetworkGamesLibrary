/*
	Created by:	Joe O'Regan
				K00203642

	SDLFunctions.h

	Initialise SDL_Rect objects
*/

#ifndef SDL_FUNCTIONS_H
#define SDL_FUNCTIONS_H

#include <SDL.h>
#include "Definitions.h"

SDL_Rect* initSDLRect(SDL_Rect *Rect, int x, int y, int w, int h);	// Initialise SDL_Rect objects
SDL_Rect makeRect(int x, int y, int w, int h);

#endif
