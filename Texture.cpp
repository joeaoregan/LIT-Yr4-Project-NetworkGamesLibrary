/*
	Joe O'Regan
	K00203642

	Texture.cpp

	Game texture functionality
*/

#include "Texture.h"
#include <iostream>

Texture* Texture::s_pInstance = 0;										// Singleton, so only one instance of texture class is used throughout the game

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

/*
	Render a button texture
*/
void Texture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip ) {	
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };							// Set rendering space and render to screen
	
	if( clip != NULL ) {											// Set clip rendering dimensions
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	
	SDL_RenderCopyEx( Game::Instance()->getRenderer(), mTexture, clip, &renderQuad, angle, center, flip );	// Render to screen
}
/*
void Texture::renderAnim(int x, int y, int frame){
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = width * currentFrame;
	srcRect.y = height * currentRow;
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x;
	destRect.y = y;

	SDL_SetTextureAlphaMod(m_textureMap[id], alpha);
	SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, angle, 0, flip);
}
*/
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

bool Texture::load(std::string fileName, std::string id) {
	SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());

	if (pTempSurface == 0) {
		std::cout << IMG_GetError();
		return false;
	}

	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(Game::Instance()->getRenderer(), pTempSurface);

	SDL_FreeSurface(pTempSurface);

	if (pTexture != 0) {
		m_TextureMap[id] = pTexture;
		return true;
	}

	return false;
}

// Array of texture details, with path, id, and description
std::string texturesL1[3][3] = {
	{ "Assets/Art/Player1Ship.png", "player1ID", "Player 1" },
	{ "Assets/Art/bg720.png", "bgID", "Background" },
	{ "Assets/Art/LaserGreen.png", "greenLaserID", "Green Laser" }
};

std::string texturesMenu[4][3] = {
	{ "Assets/Art/Menu/Logo2720.png", "logoID", "Menu Logo" },
	{ "Assets/Art/Menu/btnConnectToServer.png", "connectBtnID", "Connect To Server Button" },
	{ "Assets/Art/Menu/btnReadyToStart.png", "readyBtnID", "Ready To Play Button" },
	{ "Assets/Art/Menu/btnSinglePlayer.png", "playBtnID", "Start Game Button" }
};

/*
	Load the game textures
*/
//bool Texture::loadTextures(){
bool Texture::loadTextures(int select){
	bool success = true;
	
	if (select == MENU_DATA) {
		// Load the textures stored in the textures array
		for (int i = 0; i < sizeof(texturesMenu)/sizeof(texturesMenu[0]); i++) {
			if (!load(texturesMenu[i][0], texturesMenu[i][1])) {
				std::cout << "Failed to load " << texturesMenu[i][2] << " Menu texture!" << std::endl;
				success = false;
			}
		}
	} else if (select == LEVEL1_DATA) {
		// Load the textures stored in the textures array
		for (int i = 0; i < sizeof(texturesL1)/sizeof(texturesL1[0]); i++) {
			if (!load(texturesL1[i][0], texturesL1[i][1])) {
				std::cout << "Failed to load " << texturesL1[i][2] << " Level 1 texture!" << std::endl;
				success = false;
			}
		}
	}

	return success;
}

