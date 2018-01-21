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
	    	setX(0);
		setY(0);
		setVelX(0);
		setVelY(0);
		setWidth(100);
		setHeight(47);
		setVel(10);
		setTextureID("player1ID");
	}

	void handleInput();
	//void handleEvent( SDL_Event& e );			// Takes key presses and adjusts the Player's velocity		
	void update();						// Moves the Player		
	void render();						// Shows the Player on the screen

};

#endif	/* __PLAYER_H */
