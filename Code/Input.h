/*
	Modified by:	Joe O'Regan
					K00203642

	Originally by:	Maciej Spychala

	Input.h

	Added  keyboard keypad input
*/

#ifndef INPUT_H
#define INPUT_H

#include "GameObject.h"											// Game objects Player and Bullet

void keypadInput(int *keyPressed);								// JOR Handle input from keyboard keypad

int16_t getPlayerKeyState(Player *player);						// Apply binary OR to keystate if key is pressed
void setPlayerKeyPresses(Player *player, int16_t key_state);	// Use binary AND to set Player key presses

#endif