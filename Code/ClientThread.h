/*
	Modified by:	Joe O'Regan
					K00203642

	ClientThread.h

	Moved Client Loop here
*/

#ifndef CLIENT_THREAD_H
#define CLIENT_THREAD_H

#if defined __linux__
#include <arpa/inet.h>
#endif
#include "JOR_Net.h"
#include "GameObject.h"

int16_t arrBullets[256];								// Array of bullet objects
int getNumPlayers();									// Current number of players
int getScreenBullets();									// Current on screen bullets
int16_t getClientID();									// Client identifier
Player* getPlayers();									// Get the list of players for updating/sending data

int clientLoop(void *arg);								// JOR Client thread

#endif
