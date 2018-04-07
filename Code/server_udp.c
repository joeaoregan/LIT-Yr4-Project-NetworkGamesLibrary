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
#include <WinSock2.h>							// Windows sockets
#include <ws2tcpip.h>							// getaddrinfo()
#include <Windows.h>
#endif
#include "Time.h"

struct sockaddr_in listOfClientAddresses[MAX_PLAYERS];
struct Player listOfPlayers[MAX_PLAYERS];
struct node *listOfBullets = NULL;
int totalNumClients = 0;

void createUDPServer(int *sock, struct sockaddr_in *server_sock) {
    memset(listOfClientAddresses, 0, sizeof(struct sockaddr_in) * MAX_PLAYERS);
    if ((*sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket failed");
    }
    if (bind(*sock, (struct sockaddr*) server_sock, sizeof(struct sockaddr)) < 0) {
        perror("bind server error");
    }
}

struct sockaddr_in srvRecvfrom(int sock, int16_t data[]) {
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(struct sockaddr);
    recvfrom(sock, data, sizeof(int16_t) * 4, 0, (struct sockaddr*)&addr, &addr_size);
    return addr;
}

void srvSendto(int sock, struct sockaddr_in client, int16_t data[], int size) {
    socklen_t addr_size = sizeof(struct sockaddr);

    sendto(sock, data, sizeof(int16_t) * size, 0, (struct sockaddr*)&client, addr_size);
}

void initConnectedPlayersList() {
    int i;
    for (i = 0; i < MAX_PLAYERS; i++) {
        listOfPlayers[i].position.w = PLAYER_WIDTH;
        listOfPlayers[i].position.h = PLAYER_HEIGHT;
        listOfPlayers[i].position.x = SPAWN_X;
        listOfPlayers[i].position.y = SPAWN_Y;
    }
}

//void* server_receive_loop(void *arg) {
int serverInputLoop(void *arg) {
    int socket = *((int *) arg);																	// Socket passed in as argument
    int curClient = 0;																				// Clients position in the list of connected clients
    struct sockaddr_in cliAddr;																		// Address of the current client connection
    int16_t arrData[4];																				// Array of data received from the client
    initConnectedPlayersList();																		// Initialise the list of connected players

    while (1) {
        cliAddr = srvRecvfrom(socket, arrData);														// Receive data from client (save client address)
        curClient = findClientIDNumber(cliAddr, listOfClientAddresses, totalNumClients);			// client address, array of addresses, connected clients
        if (existingClient(curClient)) {															// If the client is an existing client
            int16_t keys = arrData[1];																// Key pressed is the 2nd position in the data array
            player_from_key_state(&listOfPlayers[curClient], keys);

            if(listOfPlayers[curClient].shoot && !listOfPlayers[curClient].reloading) {				// If the player has fired, and isn't reloading
                struct Bullet bullet;																// Create a bullet
                bullet.position.x = listOfPlayers[curClient].position.x;
                bullet.position.y = listOfPlayers[curClient].position.y;
                bullet.position.w = BULLET_WIDTH;
                bullet.position.h = BULLET_HEIGHT;
                bullet.face = listOfPlayers[curClient].face;

                if (bullet.face == 1) {																// Offset starting position for bullet (place bullet on left or right of player)
                    bullet.position.x += PLAYER_WIDTH;
                } else {
                    bullet.position.x -= BULLET_WIDTH;
                }

                bullet.player_id = curClient;														// Set the bullet id
                push_element(&listOfBullets, &bullet, sizeof(struct Bullet));						// Add bullet to the bullet list
            }

            listOfPlayers[curClient].reloading = listOfPlayers[curClient].shoot;					// Set player as reloading
        }

        if (arrData[0] == -1 && curClient < MAX_PLAYERS) {
            addClientAddrToList(curClient, &cliAddr);
            int16_t tab[3];
            tab[0] = -1;
            tab[1] = curClient;
            srvSendto(socket, listOfClientAddresses[curClient], tab, 3);
        }
		
		/*
#if defined __linux__
		usleep(50);
#elif defined _WIN32 || defined _WIN64
		Sleep(50/1000);
#endif
*/
		sleepCrossPlatform(50);

    }
	return 0;	// Changed function return type to int
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
        (*array)[1 + (i * 2)] = ((struct Bullet*) temp->data)->position.x;
        (*array)[2 + (i * 2)] = ((struct Bullet*) temp->data)->position.y;
        i++;
        temp = temp->next;
    }
    return n;
}

int serverOutputLoop(void *arg) {
    int socket = *((int *) arg);
    int16_t arrData[3];				// Data to send to the client
	Uint32 start, stop;				// JOR Replace struct timeval with Uint32 for SDL_GetTicks()
    double time_interval;
    int killer;
    while (1) {
		start = SDL_GetTicks();		// JOR Replaces gettimeofday()
        int i, j;

        updateBullets(&listOfBullets);

        for (i = 0; i < totalNumClients; i++) {
            updatePlayer(&listOfPlayers[i]);
			
            if (check_if_player_dies(&listOfPlayers[i], &listOfBullets, &killer)) {
                listOfPlayers[i].position.x = SPAWN_X;						// Repspawn shot player
                listOfPlayers[i].position.y = SPAWN_Y;
                listOfPlayers[i].deaths++;									// Increment death count for shot player
                listOfPlayers[killer].kills++;								// Increment kill count for shooting player
            }
        }

        int16_t *bullet_array = NULL;
        int bulletCount = get_bullet_array(listOfBullets, &bullet_array);

        for (i = 0; i < totalNumClients; i++) {
            for (j = 0; j < totalNumClients; j++) {
                arrData[0] = j;												// Client ID
                arrData[1] = listOfPlayers[j].position.x;					// Client X position
                arrData[2] = listOfPlayers[j].position.y;					// Client Y position
                arrData[3] = listOfPlayers[j].kills;						// Client Kills
                arrData[4] = listOfPlayers[j].deaths;						// Client deaths
                srvSendto(socket, listOfClientAddresses[i], arrData, 5);	// Send to all clients
				/*
#if defined __linux__
				usleep(20);
#elif defined _WIN32 || defined _WIN64
				Sleep(20/ 1000);
#endif
*/
				sleepCrossPlatform(20);

            } // for number_of_clients j

			//printf("bullets %d\n", bulletCount);
            srvSendto(socket, listOfClientAddresses[i], bullet_array, 1 + (bulletCount * 2));

			/*
#if defined __linux__
			usleep(20);
#elif defined _WIN32 || defined _WIN64
			Sleep(20 / 1000);
#endif
*/
			sleepCrossPlatform(20);

        } // for number_of_clients i

        free(bullet_array);

		stop = SDL_GetTicks();										// JOR Replace gettimeofday - start, stop
		time_interval = (double)(stop - start);
		if ((double)(stop - start) > 0) {
			time_interval = (double)(stop - start);
		}
		/*
#if defined __linux__
		usleep(FRAME_TIME - time_interval);
#elif defined _WIN32 || defined _WIN64
		Sleep((DWORD)((FRAME_TIME - time_interval)/1000));
#endif
*/
		sleepCrossPlatform((int) (FRAME_TIME - time_interval));
    }	// while(1)

	return 0;
}

/*
	Check is the client already in the list of connected clients
*/
int existingClient(int clientNum) {
    return (clientNum < totalNumClients && clientNum >= 0);
}

/*
	Add the client address to the list of connected clients
*/
void addClientAddrToList(int clientNum, struct sockaddr_in *cliAddr) {
    if (clientNum >= totalNumClients) {
        listOfClientAddresses[totalNumClients++] = *cliAddr;
    }
}
