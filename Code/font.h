#ifndef FONT_H
#define FONT_H

#include <SDL.h>
#include <SDL_ttf.h>

void displayTextWhite(SDL_Renderer *renderer, char *text, TTF_Font *font, int x, int y);				
void displayTextRed(SDL_Renderer *renderer, char *text, TTF_Font *font, int x, int y);
void displayText(SDL_Renderer *renderer, char *text, TTF_Font *font, int x, int y, SDL_Color color);	// Added function to set text colour

#endif
