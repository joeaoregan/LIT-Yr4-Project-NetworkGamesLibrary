/*
	Created by:	Joe O'Regan
					K00203642

	Textures.h

	Texture functions
*/

#ifndef TEXTURES_H
#define	TEXTURES_H

#include <SDL.h>

SDL_Texture* load(SDL_Renderer *renderer, char *file) {
	SDL_Surface *bitmap = SDL_LoadBMP(file);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, bitmap);
	SDL_FreeSurface(bitmap);

	return texture;
}

void renderFlip(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Rect *dest, const SDL_RendererFlip flip) {
	SDL_RenderCopyEx(renderer, texture, NULL, dest, 0, NULL, flip);
}

#endif