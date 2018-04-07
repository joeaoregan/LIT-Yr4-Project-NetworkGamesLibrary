#include "server_udp.h"
#include "network.h"
#include "objects.h"
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
#include "Time.h"
#include "SDLFunctions.h"

struct sockaddr_in listOfClientAddresses[MAX_PLAYERS];
struct Player listOfPlayers[MAX_PLAYERS];
struct node *listOfBullets = NULL;
int totalNumClients = 0;

/*
	Initilise the server socket
*/
void createUDPServer(int *sock, struct sockaddr_in *server_sock) {
    memset(listOfClientAddresses, 0, sizeof(struct sockaddr_in) * MAX_PLAYERS);				// Initialise the list of client addresses in memory
    if ((*sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {							// Initialise the server socket
        perror("Server_UDP createUDPServer: Socket Failed");
    }
	else printf("Server Socket Created: %d\n", (*sock));

    if (bind(*sock, (struct sockaddr*) server_sock, sizeof(struct sockaddr)) < 0) {			// Bind the server socket
        perror("Server_UDP createUDPServer: Bind Error");
    }
	else printf("Socket Bind OK\n\n");
}

struct sockaddr_in srvRecvfrom(int sock, int16_t data[]) {
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(struct sockaddr);
    recvfrom(sock, data, sizeof(int16_t) * 4, 0, (struct sockaddr*)&addr, &addr_size);		// Receive data from client over UDP
    return addr;
}

void srvSendto(int sock, struct sockaddr_in client, int16_t data[], int size) {
    socklen_t addr_size = sizeof(struct sockaddr);

    sendto(sock, data, sizeof(int16_t) * size, 0, (struct sockaddr*)&client, addr_size);	// Send data to client
}

void initConnectedPlayersList() {
    int i;
    for (i = 0; i < MAX_PLAYERS; i++) {
		listOfPlayers[i].position = makeRect(SPAWN_X, SPAWN_Y, PLAYER_WIDTH, PLAYER_HEIGHT);// Initialise each players position
    }
}

//void* server_receive_loop(void *arg) {
int serverInputLoop(void *arg) {
    int socket = *((int *) arg);															// Socket passed in as argument
    int curClient = 0;																		// Clients position in the list of connected clients
    struct sockaddr_in cliAddr;																// Address of the current client connection
    int16_t arrData[4];																		// Array of data received from the client
    initConnectedPlayersList();																// Initialise the list of connected players

    while (1) {
        cliAddr = srvRecvfrom(socket, arrData);												// Receive data from client (save client address)
        curClient = findClientIDNumber(cliAddr, listOfClientAddresses, totalNumClients);	// client address, array of addresses, connected clients
        if (existingClient(curClient)) {													// If the client is an existing client
            int16_t keys = arrData[1];														// Key pressed is the 2nd position in the data array
            player_from_key_state(&listOfPlayers[curClient], keys);

            if(listOfPlayers[curClient].shoot && !listOfPlayers[curClient].reloading) {		// If the player has fired, and isn't reloading
                struct Bullet bullet;														// Create a bullet
				bullet.position = makeRect(listOfPlayers[curClient].position.x,				
					listOfPlayers[curClient].position.y, BULLET_WIDTH, BULLET_HEIGHT);		// Init the player position SDL_Rect
                bullet.face = listOfPlayers[curClient].face;								// Aim the bullet in the direction the player is facing

                if (bullet.face == 1) {														// Offset starting position for bullet (place bullet on left or right of player)
                    bullet.position.x += PLAYER_WIDTH;										// Bullet start point on right of player
                } else {
                    bullet.position.x -= BULLET_WIDTH;										// Bullet start point on left of player
                }

                bullet.player_id = curClient;												// Set the bullet id
                push_element(&listOfBullets, &bullet, sizeof(struct Bullet));				// Add bullet to the bullet list
            }

            listOfPlayers[curClient].reloading = listOfPlayers[curClient].shoot;			// Set player as reloading
        }

        if (arrData[0] == -1 && curClient < MAX_PLAYERS) {									// ID field of the data array is -1, the client is new, and still under max players
            addClientAddrToList(curClient, &cliAddr);										// Add the client address to the list of connected clients
            int16_t arrData[3];																// Create a data array with 3 elements
            arrData[0] = -1;																// Keep the same client ID
            arrData[1] = curClient;															// Set the second field to the current client number
            srvSendto(socket, listOfClientAddresses[curClient], arrData, 3);				// Send data to client
        }

		sleepCrossPlatform(50);																// Sleep for 50 microseconds
    }
	return 0;																				// Changed function return type to int
}

int get_bullet_array(struct node *list, int16_t **array) {
    int n = 0;
    struct node *temp = list;
    while (temp != NULL) {
        n++;
        temp = temp->next;
    }

    *array = malloc(sizeof(int16_t) + (n * 2 * sizeof(int16_t)));
    (*array)[0] = -2;
    int i = 0; 
    temp = list;

    while (temp != NULL && i < n) {
        (*array)[1 + (i * 2)] = ((struct Bullet*) temp->data)->position.x;					// Odd array position starting at 1
        (*array)[2 + (i * 2)] = ((struct Bullet*) temp->data)->position.y;					// Even array position starting at 2
        i++;																				// increment i
        temp = temp->next;
    }

    return n;
}

int serverOutputLoop(void *arg) {
    int socket = *((int *) arg);
    int16_t arrData[3];																		// Data to send to the client
	Uint32 start, stop;																		// JOR Replace struct timeval with Uint32 for SDL_GetTicks()
    double time_interval;
    int killerID;																			// ID of the player who has killed another player
    while (1) {
		start = SDL_GetTicks();																// JOR Replaces gettimeofday(), get start time of frame
        int i, j;

        updateBullets(&listOfBullets);

        for (i = 0; i < totalNumClients; i++) {
            updatePlayer(&listOfPlayers[i]);												// --- UPDATE PLAYER ---
			
            if (check_if_player_dies(&listOfPlayers[i], &listOfBullets, &killerID)) {		// --- PLAYER DEAD ---
                listOfPlayers[i].position.x = SPAWN_X;										// Respawn shot player at Spawn position
                listOfPlayers[i].position.y = SPAWN_Y;
                listOfPlayers[i].deaths++;													// Increment death count for shot player
                listOfPlayers[killerID].kills++;											// Increment kill count for shooting player
            }
        }

        int16_t *arrBullets = NULL;															// Initialise the bullets array
        int bulletCount = get_bullet_array(listOfBullets, &arrBullets);						// Number of bullets

        for (i = 0; i < totalNumClients; i++) {
            for (j = 0; j < totalNumClients; j++) {
                arrData[0] = j;																// Client ID
                arrData[1] = listOfPlayers[j].position.x;									// Client X position
                arrData[2] = listOfPlayers[j].position.y;									// Client Y position
                arrData[3] = listOfPlayers[j].kills;										// Client Kills
				arrData[4] = listOfPlayers[j].deaths;										// Client deaths
				arrData[5] = listOfPlayers[j].flip;											// Client flip (sprite direction)
				//srvSendto(socket, listOfClientAddresses[i], arrData, 5);					// Send to all clients
				srvSendto(socket, listOfClientAddresses[i], arrData, 6);					// Send to all clients

				sleepCrossPlatform(20);														// Sleep for 20 microseconds
            } // for number_of_clients j

			//printf("bullets %d\n", bulletCount);
            srvSendto(socket, listOfClientAddresses[i], arrBullets, 1 + (bulletCount * 2));	// Send data to client

			sleepCrossPlatform(20);															// Sleep for 20 microseconds
        } // for number_of_clients i

        free(arrBullets);																	// Deallocate the bullet array from memory

		stop = SDL_GetTicks();																// JOR Replace gettimeofday - start, stop
		time_interval = (double)(stop - start);												// Calculate the length of time the frame has run for
		if ((double)(stop - start) > 0) {													// If there is time left in the frame
			time_interval = (double)(stop - start);											// Set the time interval to sleep for
		}

		sleepCrossPlatform((int) (FRAME_TIME - time_interval));								// Sleep until the end of the frame
    }	// while(1)

	return 0;
}

/*
	Check is the client already in the list of connected clients
*/
int existingClient(int clientNum) {
    return (clientNum < totalNumClients && clientNum >= 0);									// Is the client number is between 0 and the total number of clients
}

/*
	Add the client address to the list of connected clients
*/
void addClientAddrToList(int clientNum, struct sockaddr_in *cliAddr) {
    if (clientNum >= totalNumClients) {
        listOfClientAddresses[totalNumClients++] = *cliAddr;								// Add client address to the client address list
    }
}
