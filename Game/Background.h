/*
	Joe O'Regan
	K00203642

	Background.h

	Scrolling background game object
*/

#ifndef _BACKGROUND_H
#define _BACKGROUND_H

#include "GameObject.h"

class Background : public GameObject{
public:
	Background(){
		std::cout << "Background constuctor called.\n";	// Initializes the variables
		setX(0);					// Set coordinates
		setY(0);
		setWidth(1280);					// Background fills the full screen
		setHeight(600);					// Height of the background (doesn't fill window)
		setVel(-1);					// Move the background right to left
		setAlive(true);					// Set the background as active
		setTextureID("bgID");				// Set the background image
	}

	~Background() {}

	virtual void update() {
		GameObject::update();

		if (getX() <= -getWidth()) setX(0); 		// Move back to start when fully off screen
	}

	virtual void render() {
		GameObject::render();
		//SDL_Rect renderQuad = { getX(), getY(), getWidth(), getHeight() };
		SDL_Rect renderQuad2 = { getX() + getWidth(), getY(), getWidth(), getHeight() };			// Set rendering space and render to screen

		SDL_RenderCopyEx(Game::Instance()->getRenderer(), 
			Texture::Instance()->getTexture(getTextureID()), NULL, &renderQuad2, 0, NULL, SDL_FLIP_NONE);	// Render to screen
	}
};

#endif
