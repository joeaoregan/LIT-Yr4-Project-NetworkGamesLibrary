/*
	Modified by:	Joe O'Regan
					K00203642

	ServerThreads.c

	Modified server functionality to work with windows
	Then moved Network functionality to JOR_Net
*/

#include "ServerThreads.h"																	// Threads to handle server input and output
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
#include "JOR_Net.h"

struct Player listOfPlayers[JN_MAX_PLAYERS];
struct node *listOfBullets = NULL;

void initConnectedPlayersList() {
    int i;
    for (i = 0; i < JN_MAX_PLAYERS; i++) {
		listOfPlayers[i].position = makeRect(SPAWN_X, SPAWN_Y, PLAYER_WIDTH, PLAYER_HEIGHT);// Initialise each players position
    }
}

/*
	Server input thread
*/
int serverInputLoop(void *arg) {
    //int srvSock = *((int *) arg);															// Socket passed in as argument
    int curClientID = 0;																	// Clients position in the list of connected clients
    int16_t arrData[4];																		// Array of data received from the client
    initConnectedPlayersList();																// Initialise the list of connected players

    while (1) {
		curClientID = srvRecvfrom(arrData);													// Receive data from client, returning the client position in client address list

		/*
			Do existing client stuff
		*/
        if (JOR_NetExistingClient(curClientID)) {											// If the client is an existing client
            int16_t keys = arrData[1];														// Key pressed is the 2nd position in the data array
            player_from_key_state(&listOfPlayers[curClientID], keys);

            if(listOfPlayers[curClientID].shoot && !listOfPlayers[curClientID].reloading) {	// If the player has fired, and isn't reloading
                struct Bullet bullet;														// Create a bullet
				bullet.position = makeRect(listOfPlayers[curClientID].position.x,				
					listOfPlayers[curClientID].position.y, BULLET_WIDTH, BULLET_HEIGHT);	// Init the bullet at the current player position using SDL_Rect
                bullet.face = listOfPlayers[curClientID].face;								// Aim the bullet in the direction the player is facing

				/*
                if (bullet.face == 1) {														// Offset starting position for bullet (place bullet on left or right of player)
                    bullet.position.x += PLAYER_WIDTH;										// Bullet start point on right of player
                } else {
                    bullet.position.x -= BULLET_WIDTH;										// Bullet start point on left of player
                }
				*/
				bullet.position.x += (bullet.face == FORWARDS) ? PLAYER_WIDTH : -BULLET_WIDTH; // JOR


                bullet.player_id = curClientID;												// Set the bullet id to the player who fired the bullet
                push_element(&listOfBullets, &bullet, sizeof(struct Bullet));				// Add bullet to the bullet list
            }

            listOfPlayers[curClientID].reloading = listOfPlayers[curClientID].shoot;		// Set player as reloading
        }

		/*
			Do new client stuff
		*/
        if (arrData[0] == NEW_PLAYER && curClientID < JN_MAX_PLAYERS) {						// ID field of the data array is -1, the client is new, and still under max players
            int16_t arrData[3];																// Create a data array with 3 elements
            arrData[0] = NEW_PLAYER;														// Keep the same client ID
            arrData[1] = curClientID;														// Set the second field to the current client number

			srvSendto(curClientID, arrData, 3);												// Send the client its new ID
			printf("New Client ID Sent To Client\n");
        }

		JOR_NetSleep(50);																	// Sleep for 50 microseconds before firing next bullet?
    }
	return 0;																				// Changed function return type to int
}

/*
	Add the bullet positions to an array to send to the client
	Return the number of active bullets
*/
unsigned int createBulleDataArray(struct node *list, int16_t **array) {
    unsigned int numBullets = 0;															// Bullet amount Can't have negative bullets
    struct node *temp = list;																// Temporary list
    while (temp != NULL) {
        numBullets++;																		// Count the bullets
        temp = temp->next;
    }

    *array = malloc(sizeof(int16_t) + (numBullets * 2 * sizeof(int16_t)));
    (*array)[0] = BULLET;																	// Bullet ID is -2 (1st array position 0) - 0.
    unsigned int i = 0; 
    temp = list;																			// Reset list

    while (temp != NULL && i < numBullets) {
        (*array)[1 + (i * 2)] = ((struct Bullet*) temp->data)->position.x;					// Odd array position starting at 1 used to store x coordinate - 1. 
        (*array)[2 + (i * 2)] = ((struct Bullet*) temp->data)->position.y;					// Even array position starting at 2 used to store y coordinate - 2. etc...
        i++;																				// increment i
        temp = temp->next;
    }

    return numBullets;
}

int serverOutputLoop(void *arg) {
    //int socket = *((int *) arg);
    int16_t arrData[6];																		// Data to send to the client
	Uint32 start, stop;																		// JOR Replace struct timeval with Uint32 for SDL_GetTicks()
    double time_interval;
    int killerID;																			// ID of the player who has killed another player
    while (1) {
		start = SDL_GetTicks();																// JOR Replaces gettimeofday(), get start time of frame
        int i, j;

        updateBullets(&listOfBullets);

        for (i = 0; i < JOR_NetGetNumClients(); i++) {
            updatePlayer(&listOfPlayers[i]);												// --- UPDATE PLAYER ---

			
            if (check_if_player_dies(&listOfPlayers[i], &listOfBullets, &killerID)) {		// --- PLAYER DEAD ---
                listOfPlayers[i].position.x = SPAWN_X;										// Respawn shot player at Spawn position
                listOfPlayers[i].position.y = SPAWN_Y;
                listOfPlayers[i].deaths++;													// Increment death count for shot player
                listOfPlayers[killerID].kills++;											// Increment kill count for shooting player
            }
        }

        int16_t *arrBullets = NULL;															// Initialise the bullets array
        unsigned int bulletCount = createBulleDataArray(listOfBullets, &arrBullets);		// Get number of bullets, and set up bullet data to return to client

		for (i = 0; i < JOR_NetGetNumClients(); i++) {
			for (j = 0; j < JOR_NetGetNumClients(); j++) {
                arrData[0] = j;																// Client ID
                arrData[1] = listOfPlayers[j].position.x;									// Client X position
                arrData[2] = listOfPlayers[j].position.y;									// Client Y position
                arrData[3] = listOfPlayers[j].kills;										// Client Kills
				arrData[4] = listOfPlayers[j].deaths;										// Client deaths
				arrData[5] = listOfPlayers[j].flip;											// Client flip (sprite direction)

				srvSendto(i, arrData, 6);													// Send to all clients

				JOR_NetSleep(20);															// Sleep for 20 microseconds
            } // for number_of_clients j

			//if (bulletCount > 0)															// If there is bullet data to send								// BULLETS NOT CLEARING
				srvSendto(i, arrBullets, 1 + (bulletCount * 2));							// Send data to client, ID, and bullet x and y

			JOR_NetSleep(20);																// Sleep for 20 microseconds
        } // for number_of_clients i

        free(arrBullets);																	// Deallocate the bullet array from memory

		stop = SDL_GetTicks();																// JOR Replace gettimeofday - start, stop
		time_interval = (double)(stop - start);												// Calculate the length of time the frame has run for
		if ((double)(stop - start) > 0) {													// If there is time left in the frame
			time_interval = (double)(stop - start);											// Set the time interval to sleep for
		}

		JOR_NetSleep((int) (FRAME_TIME - time_interval));									// Sleep until the end of the frame
    }	// while(1)

	return 0;
}
