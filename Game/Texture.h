/*
	Joe O'Regan
	K00203642

	Texture.h

	Game texture functionality
*/

#ifndef	__TEXTURE_H
#define	__TEXTURE_H

// Unix (Ubuntu)
#if defined __linux__
#include <SDL2/SDL_image.h>
// Windows
#elif defined _WIN32 || defined _WIN64
#include <SDL_image.h>
#endif

#include "Game.h"
#include <map>

enum LoadData{
	MENU_DATA = 0,
	LEVEL1_DATA,
	PAUSE_DATA,
	GAMEOVER_DATA
};

// Texture wrapper class
class Texture {
public:	
	static Texture* Instance() {														// Single instance of Textures used throughout Game
		if (s_pInstance == 0) {
			s_pInstance = new Texture();
			return s_pInstance;
		}

		return s_pInstance;														// Make sure the texture manager only exists once
	}
	
	Texture();																// Initializes variables	
	~Texture();																// Deallocates memory

	//bool loadTextures();
	bool loadTextures(int select);

	SDL_Texture* getTexture(std::string id) { return m_TextureMap[id]; }									// Get a texture from the map using ID

	bool load(std::string fileName, std::string id);											// Load an image to the texture map given its path, assigning an ID
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
	static Texture* s_pInstance;														// Single Instance of Texture class used in Game

	std::map<std::string, SDL_Texture*> m_TextureMap;											// Map to store textures
	
	SDL_Texture* mTexture;															// The hardware texture

	int mWidth, mHeight;															// Image dimensions
};

#endif	/* __TEXTURE_H */
