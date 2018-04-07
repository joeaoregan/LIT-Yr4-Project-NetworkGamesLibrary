#ifndef FONT_H
#define FONT_H

#include <SDL.h>
#include <SDL_ttf.h>

void displayText(SDL_Renderer *renderer, char *text, TTF_Font *font, int x, int y);

#endif
