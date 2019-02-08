/*
	Modified by:	Joe O'Regan
					K00203642

	Originally by:	Maciej Spychala

	GameObject.h

	Added change in direction for player sprite
	Communicated over network, so player flips
	direction on all clients
*/

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <SDL.h>
#include "Definitions.h"										// Boolean

typedef struct PlayerObject {
    SDL_Rect position;											// Position and dimensions
    SDL_Texture *texture;										// Player sprite
    int left, right, up, down;									// Key press and direction
    int can_jump, y_speed;										// Player is positioned on an object to jump off, the players speed
    int face, shoot, reloading;									// Direction the player is facing, the player has fired, and player is reloading weapon (sets fire rate)
    int left_key, right_key, up_key, down_key, attack_key;		// Keyboard key for movement in each direction and firing
    int kills, deaths, flip;									// Change the sprite direction when movement direction changes
} Player;

typedef struct BulletObject {
    SDL_Rect position;											// Position and dimensions
    int face;													// Direction the bullet is to move in
    int player_id;												// Identifies the player who fired the bullet
} Bullet;

void initPlayer(Player *players);								// Initialise the player struct

bool isKeyUp(int key, Player* player);							// Return true if all game keys are up NOT CORRECT - should return true each key
bool isKeyDown(int key, Player* player);						// Return true if any game key is pressed
bool getInputFromPlayer(SDL_Event e, Player* player);			// Get keyboard input and set the player saved key presses
Bullet initBullet(int x, int y, int face);						// Initialise bullet objects, added check for player face, to make sure bullet fires from initial position

#endif
