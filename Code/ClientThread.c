/*
	Modified by:	Joe O'Regan
					K00203642

	ClientThread.c

	Moved Client Loop here
*/

#include "ClientThread.h"														// Threads to handle server input and output
#include "Input.h"																// Handle player keyboard input
#include "GameObject.h"															// Game objects Player and Bullet
#include "list.h"																// Linked list functions
#include "physic.h"
#include "Definitions.h"														// Game defines
#include "time.h"
#if defined __linux__
#include "sys/time.h"
#elif defined _WIN32 || defined _WIN64
#include <WinSock2.h>															// Windows sockets
#include <ws2tcpip.h>															// getaddrinfo()
#include <Windows.h>
#endif
#include "SDLFunctions.h"

Player playerList[JN_MAX_PLAYERS];												// Player array
Player* getPlayers() { return playerList; }										// Get the Player array

int numPlayers = 0;																// Number of players currently in the game
int getNumPlayers() { return numPlayers; }										// Get the current number of players

int totalBulletsOnScreen = 0;													// Current bullet
int getScreenBullets() { return totalBulletsOnScreen; }							// Get the number of bullets on screen

int16_t cliID = NEW_PLAYER;														// Initialise client ID to -1
int16_t getClientID() { return cliID; }											// Get the client ID

int clientLoop(void *arg) {
	int16_t arrData[JN_BUF_MAX];												// Data to receive from server
	int numBytes, id, bulletsInArray;											// Number of bytes received , clientID, Active bullets
	bool idSet = false;															// The client ID has not been set yet

	initPlayer(playerList);														// Initialise the list of players

	while (1) {
		/*
			Get client ID from data received
		*/
		numBytes = JOR_NetCliRecvfrom(arrData);									// Receive data from server
		id = arrData[0];														// Parse received data, first int = id
		
		/*
			Assign ID to new client connection
		*/
		if (id == NEW_PLAYER && !idSet) {										// Parse data when the ID is unset
			JOR_NetSetClientID(arrData[1], &cliID, &numPlayers);				// Assign an ID for the connected player
			idSet = true;														// Stops the ID being set more than once
		}

		/*
			Receive update from server for existing player
		*/
		if (id >= 0) {															// Parse existing Client data
			//if (JOR_NetCheckNewClient(id, &numPlayers))						// Increase number of players if new player added		
				//printf("New Connection, %s \n", JOR_NetDisplayClientAddr(id));
				//JOR_NetTextColour("New Connection\n", BLUE);					// Display new connection text in blue to highlight

			playerList[id].position.x = arrData[1];								// Player x position
			playerList[id].position.y = arrData[2];								// Player y position
			playerList[id].kills = arrData[3];									// Number of kills
			playerList[id].deaths = arrData[4];									// Number of times died
			playerList[id].flip = arrData[5];									// Client flip (sprite direction)
		}

		/*
			Receive server update for bullets
		*/
		if (id == BULLET) {														// Parse Bullet data
			bulletsInArray = (numBytes - SIZE16) / (SIZE16 * BUL_DATA);			// Number of bullets in bullet list, take away the ID byte then divide by data bytes for each bullet

			memcpy(arrBullets, arrData + 1, SIZE16 * BUL_DATA * bulletsInArray);// Copy the number of bytes to the memory location

			if (totalBulletsOnScreen != bulletsInArray) {
				JOR_NetTextColour("Bullet Total: ", GREEN);						// Highlight Bullet total text as green
				printf("%d\n", totalBulletsOnScreen);							// Display a count of the number of bullets on screen
			}

			totalBulletsOnScreen = bulletsInArray;
		}

		JOR_NetSleep(50);														// Sleep for 50 microseconds
	}

	return 0;																	// Changed function return type to remove incompatible pointer type warning
}
