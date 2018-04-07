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

struct sockaddr_in receive_data(int sock, int16_t data[]) {
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(struct sockaddr);
    recvfrom(sock, data, sizeof(int16_t) * 4, 0, (struct sockaddr*)&addr, &addr_size);
    return addr;
}

void send_data(int sock, struct sockaddr_in client, int16_t data[], int size) {
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
int server_receive_loop(void *arg) {
    int socket = *((int *) arg);
    int curClient = 0;
    struct sockaddr_in cliAddr;
    int16_t arrData[4];
    initConnectedPlayersList();
    while (1) {
        cliAddr = receive_data(socket, arrData);
        curClient = addr_pos_in_tab(cliAddr, listOfClientAddresses, totalNumClients);
        if (existingClient(curClient)) {
            int16_t keys = arrData[1];
            player_from_key_state(&listOfPlayers[curClient], keys);
            if(listOfPlayers[curClient].shoot && !listOfPlayers[curClient].reloading) {
                struct Bullet temp;
                temp.position.x = listOfPlayers[curClient].position.x;
                temp.position.y = listOfPlayers[curClient].position.y;
                temp.position.w = BULLET_WIDTH;
                temp.position.h = BULLET_HEIGHT;
                temp.face = listOfPlayers[curClient].face;
                if (temp.face == 1) {
                    temp.position.x += PLAYER_WIDTH;
                } else {
                    temp.position.x -= BULLET_WIDTH;
                }
                temp.player_id = curClient;
                push_element(&listOfBullets, &temp, sizeof(struct Bullet));
            }
            listOfPlayers[curClient].reloading = listOfPlayers[curClient].shoot;
        }
        if (arrData[0] == -1 && curClient < MAX_PLAYERS) {
            addClientAddrToList(curClient, &cliAddr);
            int16_t tab[3];
            tab[0] = -1;
            tab[1] = curClient;
            send_data(socket, listOfClientAddresses[curClient], tab, 3);
        }
		
#if defined __linux__
		usleep(50);
#elif defined _WIN32 || defined _WIN64
		Sleep(50/1000);
#endif

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

int server_send_loop(void *arg) {
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
                listOfPlayers[i].position.x = SPAWN_X;
                listOfPlayers[i].position.y = SPAWN_Y;
                listOfPlayers[i].deaths++;
                listOfPlayers[killer].kills++;
            }
        }

        int16_t *bullet_array = NULL;
        int bullets_n = get_bullet_array(listOfBullets, &bullet_array);

        for (i = 0; i < totalNumClients; i++) {
            for (j = 0; j < totalNumClients; j++) {
                arrData[0] = j;
                arrData[1] = listOfPlayers[j].position.x;
                arrData[2] = listOfPlayers[j].position.y;
                arrData[3] = listOfPlayers[j].kills;
                arrData[4] = listOfPlayers[j].deaths;
                send_data(socket, listOfClientAddresses[i], arrData, 5);
				
#if defined __linux__
				usleep(20);
#elif defined _WIN32 || defined _WIN64
				Sleep(20/ 1000);
#endif

            } // for number_of_clients j

            send_data(socket, listOfClientAddresses[i], bullet_array, 1 + (bullets_n * 2));
			
#if defined __linux__
			usleep(20);
#elif defined _WIN32 || defined _WIN64
			Sleep(20 / 1000);
#endif

        } // for number_of_clients i

        free(bullet_array);

		stop = SDL_GetTicks();										// JOR Replace gettimeofday - start, stop
		time_interval = (double)(stop - start);
		if ((double)(stop - start) > 0) {
			time_interval = (double)(stop - start);
		}

#if defined __linux__
		usleep(FRAME_TIME - time_interval);
#elif defined _WIN32 || defined _WIN64
		Sleep((DWORD)((FRAME_TIME - time_interval)/1000));
#endif


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
