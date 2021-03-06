/*
	Modified by:	Joe O'Regan
					K00203642

	Originally by:	Maciej Spychala

	Definitions.h

	Moved network functionality to JOR_Net library
	Added text centering
*/

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "stdbool.h"

/*
	JOR Additions
*/
#define DEBUG_KEYSTATE true						// Debug the key presses
#define DEBUG_INPUT_LOOP_COUNT false				// Main send loop
#define DEBUG_MAIN_COORDS false						// Main coordinate check

#define BUL_DATA 3									// Data for each bullet: x, y, and ID

#define CENTRE_TEXT -1								// JOR If used for x coordinate centre horizonally, and y vertical
#define SIZE16 sizeof(int16_t)						// JOR Size of int16_t type
#define BULLET_SPEED 5								// Speed of the bullets

enum messageID { NEW_PLAYER = -1, BULLET = -2 };	// Decides how the data is to be processed
enum direction { FORWARDS = 1, BACKWARDS = -1 };	// Player sprite facing, and weapons firing direction

/*
	Original Game
*/
#define SCREEN_WIDTH 640							// Screen Dimensions
#define SCREEN_HEIGHT 480

#define FRAME_TIME 16666							// Used to calculate frame rate
#define TILE_SIZE 32								// Tile width and height

// Player
#define GRAVITY 1									// Player jumping, gravity and movment
#define MAX_VERTICAL_SPEED 25
#define PLAYER_JUMP_POWER 25
#define PLAYER_SPEED 4
#define PLAYER_WIDTH 16								// Player dimensions
#define PLAYER_HEIGHT 16						
#define BULLET_HEIGHT 8								// Bullet dimensions
#define BULLET_WIDTH 8
#define SPAWN_X 310									// Spawn point coordinates
#define SPAWN_Y 10

#define CLIENT_FRAME_TIME 15
#define SERVER_FRAME_TIME 15

#define X_AXIS 1
#define Y_AXIS 0

// Key presses
#define LEFT_KEY 0x01								// Client acts as a dumb terminal sending key presses to server
#define RIGHT_KEY 0x02
#define UP_KEY 0x04
#define DOWN_KEY 0x08
#define ATTACK_KEY 0x10

#endif

