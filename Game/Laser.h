/*
	Joe O'Regan
	K00203642

	Laser.h 

	Functionality for laser objects
*/
#ifndef LASER_H
#define LASER_H

//#include "Socket.h"
#include "GameObject.h"
#include "Texture.h"
#include <iostream>

class Laser : public GameObject {
public:
	Laser() {
		std::cout << "Laser constuctor called.\n";	// Initializes the variables
		setWidth(50);
		setHeight(5);
		setVel(20);
		setAlive(true);
		setTextureID("greenLaserID");
		setName("Laser");
	}	
			
	~Laser() {
		std::cout << "Laser destructor called.\n";
	}
	
	void spawn(int x, int y, int velocity);
	void update();						// Moves the laser	
	virtual void render();					// Shows the laser on the screen
};

#endif
