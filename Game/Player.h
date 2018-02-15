/*
	Joe O'Regan
	K00203642

	Player.h

	Player game object functionality
*/

#ifndef	__PLAYER_H
#define	__PLAYER_H

//#include "Game.h"
#include "GameObject.h"

// The Player that will move around on the screen
class Player : public GameObject {
public:	
	Player() {
	    	//Initialize the offsets
	    	setX(0);					// X coordinate
		setY(0);					// Y coordinate
		setVelX(0);					// Velocity on X axis
		setVelY(0);					// Velocity on Y axis
		setWidth(100);					// Player sprite width (in pixels)
		setHeight(47);					// Player sprite height
		setVel(10);					// Overall velocity
		setTextureID("player1ID");			// Texture to use for player sprite	SET THE TEXTURE ID BASED ON THE NET ID (1 = player1, 2 = player 2, etc.)
		setNetID(0);					// Identify the player over the network
	}

	void handleInput();
	//void handleEvent( SDL_Event& e );			// Takes key presses and adjusts the Player's velocity		
	void update();						// Moves the Player		
	void render();						// Shows the Player on the screen

};

#endif	/* __PLAYER_H */
