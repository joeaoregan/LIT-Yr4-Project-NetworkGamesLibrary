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

void Laser::move() {
	GameObject::move();

	setX(getX() + getVel());			// Move the ship left or right

	// destroy laser beam once it is offscreen
	if (getX() > SCREEN_WIDTH) {
		setAlive(false);
		Game::Instance()->netDestroyGameObject();
		//delete this;

		//sendToServer("4 Laser_Destroyed");
		//if (getAlive()) 
		//	printf("test laser alive");
	}
	else setAlive(true);
}
