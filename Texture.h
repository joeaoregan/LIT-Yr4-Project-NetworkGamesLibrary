/*
	Joe O'Regan
	K00203642

	Texture.h

	Game texture functionality
*/

#ifndef	__TEXTURE_H
#define	__TEXTURE_H

#include "Game.h"
#include <SDL2/SDL_image.h>

// Texture wrapper class
class Texture {
public:
	
	Texture();																// Initializes variables
	
	~Texture();																// Deallocates memory

	bool loadFromFile( std::string path );													// Loads image at specified path
	
	#ifdef _SDL_TTF_H	
	bool loadFromRenderedText( std::string textureText, SDL_Color textColor );								// Creates image from font string
	#endif
	
	void free();																// Deallocates texture
	
	void setColor( Uint8 red, Uint8 green, Uint8 blue );											// Set color modulation

	void setBlendMode( SDL_BlendMode blending );												// Set blending
	
	void setAlpha( Uint8 alpha );														// Set alpha modulation
		
	void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );// Renders texture at given point


	//TTF_Font *gFont = NULL;														// Globally used font

	// Gets image dimensions
	int getWidth() { return mWidth; }													// Return the texture width
	int getHeight() { return mHeight; }													// Return the texture height

private:
	
	SDL_Texture* mTexture;															// The hardware texture

	int mWidth, mHeight;															// Image dimensions
};

#endif	/* __TEXTURE_H */
