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

//struct Player players[JN_MAX_PLAYERS];
int16_t arrBullets[256];				// Array of bullet objects

//struct Player getPlayerList() { return *players; }
int getNumPlayers();
int getScreenBullets();

int clientLoop(void *arg);				// JOR Client thread

#endif
