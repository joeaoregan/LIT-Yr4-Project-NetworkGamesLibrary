/*
	Created by:	Joe O'Regan
					K00203642

	Textures.h

	Texture functions

	Load textures
	Change the direction the player sprite is facing
*/

#ifndef TEXTURES_H
#define	TEXTURES_H

#include <SDL.h>

/*
	Load the speciified texture
*/
SDL_Texture* load(SDL_Renderer *renderer, char *file) {
	SDL_Surface *bitmap = SDL_LoadBMP(file);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, bitmap);
	SDL_FreeSurface(bitmap);

	return texture;
}

/*
	Flip the texture in the specified direction
	Used for changing the direction of the player sprite
*/
void renderFlip(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Rect *dest, const SDL_RendererFlip flip) {
	SDL_RenderCopyEx(renderer, texture, NULL, dest, 0, NULL, flip);
}

#endif