#ifndef _BACKGROUND_H
#define _BACKGROUND_H

#include "GameObject.h"

class Background : public GameObject{
public:
	Background(){
		std::cout << "Background constuctor called.\n";	// Initializes the variables
		setX(0);
		setY(0);
		setWidth(1280);
		setHeight(600);
		setVel(-1);
		setAlive(true);
		setTextureID("bgID");


	}

	~Background() {}

	virtual void move() {
		GameObject::move();

		if (getX() <= -getWidth()) setX(0); 		// Move back to start when fully off screen
	}

	virtual void render() {
		SDL_Rect renderQuad = { getX(), getY(), getWidth(), getHeight() };
		SDL_Rect renderQuad2 = { getX() + getWidth(), getY(), getWidth(), getHeight() };								// Set rendering space and render to screen
		SDL_RenderCopyEx(Game::Instance()->getRenderer(), Texture::Instance()->getTexture(getTextureID()), NULL, &renderQuad, 0, NULL, SDL_FLIP_NONE);	// Render to screen

		SDL_RenderCopyEx(Game::Instance()->getRenderer(), Texture::Instance()->getTexture(getTextureID()), NULL, &renderQuad2, 0, NULL, SDL_FLIP_NONE);	// Render to screen
	}

//private: 
//	int scrollingOffset;
};

#endif
