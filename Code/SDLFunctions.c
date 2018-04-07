#include "SDLFunctions.h"

/*
	Initialise SDL_Rects
*/
SDL_Rect* initSDLRect(SDL_Rect *Rect, int x, int y, int w, int h) {
	Rect->x = x;
	Rect->y = y;
	Rect->h = h;
	Rect->w = w;

	return Rect;
}
SDL_Rect makeRect(int x, int y, int w, int h) {
	SDL_Rect rect = { x, y, w, h };
	return rect;
}