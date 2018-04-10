/*
	Modified by:	Joe O'Regan
					K00203642

	Definitions.h

	Moved network functionality to JOR_Net library
	Added text centering
*/

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "stdbool.h"

// Text
#define CENTRE_TEXT -1						// JOR If used for x coordinate centre horizonally, and y vertical

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FRAME_TIME 16666
#define TILE_SIZE 32
#define GRAVITY 1
#define MAX_VERTICAL_SPEED 25
#define PLAYER_JUMP_POWER 25
#define PLAYER_SPEED 4
#define BULLET_SPEED 5						// Speed of the bullets
#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 16
#define BULLET_HEIGHT 8
#define BULLET_WIDTH 8
#define CLIENT_FRAME_TIME 15
#define SERVER_FRAME_TIME 15
#define X_AXIS 1
#define Y_AXIS 0
//#define BUF_MAX 256
#define SPAWN_X 310
#define SPAWN_Y 10
#define LEFT_KEY 0x01
#define RIGHT_KEY 0x02
#define UP_KEY 0x04
#define DOWN_KEY 0x08
#define ATTACK_KEY 0x10

#endif
