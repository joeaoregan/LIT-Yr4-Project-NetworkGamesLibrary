/*
	Joe O'Regan
	K00203642

	Laser.cpp 

	Implementations of Laser functions
*/

//#pragma once

#include "Laser.h"
#include "Player.h"
//#include "Socket.h"
#include "Game.h"

void Laser::spawn(int x, int y, int velocity) {
	setX(x);
	setY(y);
	setVel(velocity);
}

void Laser::update() {
	GameObject::update();

	setX(getX() + getVel());			// Move the ship left or right

	// destroy laser beam once it is offscreen
	if (getX() > SCREEN_WIDTH) {
		setAlive(false);
		//Game::Instance()->netDestroyGameObject();
		//delete this;

		//sendToServer("4 Laser_Destroyed");
		//if (getAlive()) 
		//	printf("test laser alive");
	}
	else setAlive(true);
}

void Laser::render() {
	SDL_Rect renderQuad = { getX(), getY(), getWidth(), getHeight() };	// Set rendering space and render to screen

	//SDL_RenderCopyEx(Game::Instance()->getRenderer(), Texture::Instance()->getTexture(getTextureID()), NULL, &renderQuad, getAngle(), NULL, SDL_FLIP_NONE);	// Render to screen
	SDL_RenderCopyEx(Game::Instance()->getRenderer(), Texture::Instance()->getTexture(getTextureID()), NULL, &renderQuad, 0, NULL, SDL_FLIP_NONE);	// Render to screen
};
