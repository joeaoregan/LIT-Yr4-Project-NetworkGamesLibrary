/*
	Modified by:	Joe O'Regan
					K00203642

	Originally by:	Maciej Spychala

	Input.h

	Added  keyboard keypad input
*/

#ifndef INPUT_H
#define INPUT_H

/*
	Display key state as binary
	Stack Overflow User: William Whyte, July 2010
*/
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c\n"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

#include "GameObject.h"											// Game objects Player and Bullet

void keypadInput(int *keyPressed);								// JOR Handle input from keyboard keypad

int16_t getPlayerKeyState(Player *player);						// Apply binary OR to keystate if key is pressed
void setPlayerKeyPresses(Player *player, int16_t key_state);	// Use binary AND to set Player key presses

#endif