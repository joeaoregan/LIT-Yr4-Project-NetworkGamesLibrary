/*
	Modified by:	Joe O'Regan
					K00203642

	Input.h

	Added  keyboard keypad input
*/

#ifndef INPUT_H
#define INPUT_H

#include "GameObject.h"													// Game objects Player and Bullet

int16_t key_state_from_player(Player *player);
void player_from_key_state(Player *player, int16_t key_state);
void keypadInput(int *keyPressed);										// JOR Handle input from keyboard keypad

#endif