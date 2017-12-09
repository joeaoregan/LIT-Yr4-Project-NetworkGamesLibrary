#include "Texture.h"


Texture::Texture() {
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

Texture::~Texture() {	
	free();													// Deallocate
}
	
void Texture::free() {	
	if( mTexture != NULL ) {										// Free texture if it exists
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void Texture::setColor( Uint8 red, Uint8 green, Uint8 blue ) {	
	SDL_SetTextureColorMod( mTexture, red, green, blue );							// Modulate texture rgb
}

void Texture::setBlendMode( SDL_BlendMode blending ) {	
	SDL_SetTextureBlendMode( mTexture, blending );								// Set blending function
}
		
void Texture::setAlpha( Uint8 alpha ) {	
	SDL_SetTextureAlphaMod( mTexture, alpha );								// Modulate texture alpha
}

void Texture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip ) {	
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };							// Set rendering space and render to screen
	
	if( clip != NULL ) {											// Set clip rendering dimensions
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	
	SDL_RenderCopyEx( Game::Instance()->getRenderer(), mTexture, clip, &renderQuad, angle, center, flip );	// Render to screen
}

bool Texture::loadFromFile( std::string path ) {	
	free();													// Get rid of preexisting texture
	
	SDL_Texture* newTexture = NULL;										// The final texture

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL ) {
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	} else {		
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );	// Color key image
		
       		newTexture = SDL_CreateTextureFromSurface( Game::Instance()->getRenderer(), loadedSurface );	// Create texture from surface pixels

		if( newTexture == NULL ) {
			printf( "Unable to create texture: %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		} else {			
			mWidth = loadedSurface->w;								// Get image width
			mHeight = loadedSurface->h;								// and height
		}
		
		SDL_FreeSurface( loadedSurface );								// Get rid of old loaded surface
	}

	// Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool Texture::loadFromRenderedText( std::string textureText, SDL_Color textColor ) {	
	free();													// Get rid of preexisting texture

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( Game::Instance()->getFont(), textureText.c_str(), textColor );
	if( textSurface != NULL ) {
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( Game::Instance()->getRenderer(), textSurface );
		if( mTexture == NULL ) {
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		} else {			
			mWidth = textSurface->w;								// Get image width
			mHeight = textSurface->h;								// and height
		}
		
		SDL_FreeSurface( textSurface );									// Get rid of old surface
	}
	else {
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	
	return mTexture != NULL;										// Return success
}
#endif


