/*
	Modified by:	Joe O'Regan
					K00203642

	ClientThread.c

	Moved Client Loop here
*/

#include "ClientThread.h"																	// Threads to handle server input and output
#include "Input.h"																			// Handle player keyboard input
#include "GameObject.h"																		// Game objects Player and Bullet
#include "list.h"
#include "physic.h"
#include "Definitions.h"
#include "time.h"
#if defined __linux__
#include "sys/time.h"
#elif defined _WIN32 || defined _WIN64
#include <WinSock2.h>																		// Windows sockets
#include <ws2tcpip.h>																		// getaddrinfo()
#include <Windows.h>
#endif
#include "SDLFunctions.h"
//#include "Game.h"

struct Player players[JN_MAX_PLAYERS];

struct Player* getPlayers() { return &players; }

int numPlayers = 0;						// Number of players currently in the game
int getNumPlayers() { return numPlayers; }

int totalBulletsOnScreen = 0;			// Current bullet
int getScreenBullets() { return totalBulletsOnScreen; }

int16_t cliID = -1;
int16_t* getClientID() { return cliID; }

///int count = 1;

//struct Player *players = getPlayers();


int clientLoop(void *arg) {
	initPlayer(players);																						// Initialise the list of players

	printf("Players initilised\n");
	//printf("START - loop times %d\n", count++);

	//struct Player *players = ((struct Player *)arg);															// cliSock passed in as argument

	int16_t arrData[JN_BUF_MAX];																				// Data to receive from server
	int numBytes, id, bulletsInArray;																			// Number of bytes received , clientID, Active bullets
	bool idSet = false;																							// The client ID has not been set yet




	while (1) {
		//printf("loop times %d\n", count++);

		//numBytes = cliRecvfrom(socket, arrData);																// Receive data from server
		numBytes = cliRecvfrom(arrData);																		// Receive data from server

		id = arrData[0];																						// Parse received data, first int = id
		printf("RECEIVED FROM SERV id %d arrData[1] %d\n", id, arrData[1]);

		if (id == -1 && !idSet) {																				// Parse data when the ID is unset
		//if (id == -1) {																						// Parse data when the ID is unset
			//printf("test\n");
			JOR_NetSetClientID(arrData[1], &cliID, &numPlayers);												// xxx Assign an id for the connected player
			printf("cliId %d\n", cliID);
			idSet = true;																						// Stops the ID being set more than once
		}

		printf("after id set\n");








		if (id >= 0) {																							// Parse existing Client data
			JOR_NetCheckNewClient(id, &numPlayers);																// Increase number of players if new player added
			
			printf("JOR_NetCheckNewClient id %d numPlayers %d\n", id, numPlayers);

			players[id].position.x = arrData[1];																// Player x position
			players[id].position.y = arrData[2];																// Player y position
			players[id].kills = arrData[3];																		// Number of kills
			players[id].deaths = arrData[4];																	// Number of times died
			players[id].flip = arrData[5];																		// Client flip (sprite direction)
		}

		if (id == -2) {																							// Parse Bullet data
			bulletsInArray = (numBytes - sizeof(int16_t)) / (sizeof(int16_t) * 2);								// Number of bullets in bullet list
			memcpy(arrBullets, arrData + 1, sizeof(int16_t) * 2 * bulletsInArray);
			if (totalBulletsOnScreen != bulletsInArray) printf("Bullet Total: %d\n", totalBulletsOnScreen);		// Display a count of the number of bullets on screen
			totalBulletsOnScreen = bulletsInArray;
		}

		JOR_NetSleep(50);																						// Sleep for 50 microseconds
	}

	Sleep(1000000);

	return 0;																									// Changed function return type to remove incompatible pointer type warning
}