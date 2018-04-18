/*
	Modified by:	Joe O'Regan
					K00203642

	Originally by:	Maciej Spychala

	Text.h

	Added options to select text colour and centre horizontally and vertically
	Added text centering on screen
	Added colour selection and red and white text functions
*/


#ifndef TEXT_H
#define TEXT_H

#include <SDL.h>
#include <SDL_ttf.h>

void displayTextWhite(SDL_Renderer *renderer, char *text, TTF_Font *font, int x, int y);				// Display white text			
void displayTextRed(SDL_Renderer *renderer, char *text, TTF_Font *font, int x, int y);					// Display red text
void displayText(SDL_Renderer *renderer, char *text, TTF_Font *font, int x, int y, SDL_Color color);	// Added function to set text colour

#endif