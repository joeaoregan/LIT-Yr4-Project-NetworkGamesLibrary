#include "font.h"
#include "SDLFunctions.h"

/*
	Display text with the font and position specified
*/
void displayText(SDL_Renderer *renderer, char *text, TTF_Font *font, int x, int y) {
    SDL_Color color = {255, 255, 255};												// Colour is white
	SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);					// Create the text surface
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);			// Create the text texture
	SDL_Rect pos = makeRect(x, y, surface->w, surface->h);							// Initialise text position SDL_Rect
    SDL_FreeSurface(surface);														// Deallocate the surface
    SDL_RenderCopy(renderer, texture, NULL, &pos);									// Render the text
}
