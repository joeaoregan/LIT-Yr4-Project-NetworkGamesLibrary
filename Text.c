/*
	Modified by:	Joe O'Regan
					K00203642

	Originally by:	Maciej Spychala

	Text.c

	Added options to select text colour and centre horizontally and vertically
	Added text centering on screen
	Added colour selection and red and white text functions
*/

#include "Text.h"																						// Display game text
#include "SDLFunctions.h"																				// Init SDL Rects
#include "Definitions.h"

/*
	Display white text with the font and position specified
*/
void displayTextWhite(SDL_Renderer *renderer, char *text, TTF_Font *font, int x, int y) {
	SDL_Color color = { 255, 255, 255 };																// Colour is white
	displayText(renderer, text, font, x, y, color);
}

/*
	Display red text with the font and position specified
*/
void displayTextRed(SDL_Renderer *renderer, char *text, TTF_Font *font, int x, int y) {
	SDL_Color color = { 255, 0, 0 };																	// Colour is red
	displayText(renderer, text, font, x, y, color);
}

/*
	Added colour selection and centre text options for displaying text
*/
void displayText(SDL_Renderer *renderer, char *text, TTF_Font *font, int x, int y, SDL_Color color) {
	SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);										// Create the text surface
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);								// Create the text texture

	if (x == CENTRE_TEXT) x = (SCREEN_WIDTH / 2) - (surface->w / 2);									// JOR Centre the text horizontally on screen if x = -1
	if (y == CENTRE_TEXT) y = (SCREEN_HEIGHT / 2) - (surface->h / 2);									// JOR Centre the text vertrically on screen if y = -1

	SDL_Rect pos = makeRect(x, y, surface->w, surface->h);												// Initialise text position SDL_Rect
	SDL_FreeSurface(surface);																			// Deallocate the surface
	SDL_RenderCopy(renderer, texture, NULL, &pos);														// Render the text
}