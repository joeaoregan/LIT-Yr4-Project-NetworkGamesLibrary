/*
	Joe O'Regan
	K00203642

	Laser.h 

	Functionality for laser game objects
*/

#ifndef LASER_H
#define LASER_H

#include <iostream>
#include "GameObject.h"
#include "Texture.h"
#include "Player.h"
#include "Game.h"
//#include "Socket.h"

class Laser : public GameObject {
public:
	Laser() {
		std::cout << "Laser constuctor called.\n";				// Initializes the variables
		setWidth(50);								// Set width of laser to 50 pixels
		setHeight(5);								// Set height of laser to 5 pixels
		setVel(20);								// Velocity of the laser
		setAlive(true);								// Set the laser active
		setTextureID("greenLaserID");						// Use green laser image for texture
		setName("Laser");							// Set identifier for debugging etc.
	}	
			
	~Laser() {
		std::cout << "Laser destructor called.\n";
	}
	
	void spawn(int x, int y, int velocity) {
		setX(x);
		setY(y);
		setVel(velocity);
	}

	void update() {									// Moves the laser
		GameObject::update();

		setX(getX() + getVel());						// Move the ship left or right

		// destroy laser beam once it is offscreen
		if (getX() > SCREEN_WIDTH) {
			setAlive(false);
			//Game::Instance()->netDestroyGameObject();
			//delete this;
			//sendToServer("4 Laser_Destroyed");
			//if (getAlive()) printf("test laser alive");
		}
		else setAlive(true);
	}	
								
	virtual void render() {								// Shows the laser on the screen
		SDL_Rect renderQuad = { getX(), getY(), getWidth(), getHeight() };	// Set rendering space and render to screen
		SDL_RenderCopyEx(Game::Instance()->getRenderer(), 
			Texture::Instance()->getTexture(getTextureID()), 
			NULL, &renderQuad, getAngle(), NULL, SDL_FLIP_NONE);		// Render to screen
	}
};

#endif
