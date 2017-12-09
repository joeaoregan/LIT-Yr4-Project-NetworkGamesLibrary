/*
	Laser.cpp 
	implementations of Laser functions
*/

#include "Laser.h"
#include "Player.h"

void Laser::spawn(int x, int y, int velocity) {
	setX(x);
	setY(y);
	setVel(velocity);
}

void Laser::move() {
	GameObject::move();

	setX(getX() + getVel());			// Move the ship left or right

	// destroy laser beam once it is offscreen
	if (getX() > SCREEN_WIDTH) setAlive(false);
	else setAlive(true);
}
