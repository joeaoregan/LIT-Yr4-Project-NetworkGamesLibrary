#include "network.h"
#include "Definitions.h"

/* 
// IN JOR NET LIB

void initWinsock() {
#if defined _WIN32 || defined _WIN64
	WSADATA wsa;																							// Windows sockets implementation structure
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {															// If winsock doesn't initialise
		printf("Failed. Error Code : %d", WSAGetLastError());												// Display an error message
		exit(EXIT_FAILURE);																					// And exit with the specified error code
	}
	printf("Initialised Winsock.\n");																		// Otherwise indicate winsock has initialised
#endif
}
*/
/*
struct sockaddr_in intServerAddr(char *ip) {
    struct sockaddr_in srvAddr;
    memset(&srvAddr, 0, sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;

	// If the IP address was not entered from a client set to INADDR_ANY
    if (ip == NULL) {
		//srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
#if defined __linux__
		srvAddr.sin_addr.s_addr = INADDR_ANY;																// Linux Server: listen on any local address
#elif defined _WIN32 || defined _WIN64
		inet_pton(AF_INET, SERV_ADDR, &srvAddr.sin_addr);													// Windows Server: Specify the address as 127.0.0.1
#endif
    } else {
		inet_pton(AF_INET, ip, &srvAddr.sin_addr);															// JOR Replace inet_aton with inet_pton
    }

    srvAddr.sin_port = htons(SERV_PORT);

	char str[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET, &(srvAddr.sin_addr), str, INET6_ADDRSTRLEN);
	printf("server_sock_addr Server Address: %s:%d\n", str, ntohs(srvAddr.sin_port));

    return srvAddr;
}
*/
/*
struct sockaddr_in initClientAddr() {
    struct sockaddr_in cliAddr;
    memset(&cliAddr, 0, sizeof(struct sockaddr));
    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = INADDR_ANY;
    cliAddr.sin_port = 0;
    return cliAddr;
}
*/

/*
	Return the clients position in the list or the next position in the lists of clients for new client

int findClientIDNumber(struct sockaddr_in newCliAddr, struct sockaddr_in clientsList[], int numClients) {
    int i;
    for (i = 0; i < numClients; i++) {
        if (compare_addr(&newCliAddr, &clientsList[i])) {													// If the address matches and address in the existing clients
            return i;																						// Return clients position in the list									
        }
    }
    return numClients;																						// Return next client ID number
}
int compare_addr(struct sockaddr_in *a, struct sockaddr_in *b) {
    if (a->sin_port == b->sin_port &&
            a->sin_family == b->sin_family &&
            a->sin_addr.s_addr == b->sin_addr.s_addr) {
        return true;
    } else {
        return false;
    }
}
*/

int16_t key_state_from_player(struct Player *player) {
	//printf("key_state from player\n");
    int16_t key_state = 0;
    if (player->left) {
        key_state = key_state | LEFT_KEY;
    }
    if (player->right) {
        key_state = key_state | RIGHT_KEY;
    }
    if (player->up) {
        key_state = key_state | UP_KEY;
    }
    if (player->down) {
        key_state = key_state | DOWN_KEY;
    }
    if (player->shoot) {
        key_state = key_state | ATTACK_KEY;
    }
    return key_state;
}

void player_from_key_state(struct Player *player, int16_t key_state) {	
	//printf("player from key state\n");
    if (key_state & LEFT_KEY) {
        player->left = true;
    } else {
        player->left = false;
    }
    if (key_state & RIGHT_KEY) {
        player->right = true;
    } else {
        player->right = false;
    }
    if (key_state & UP_KEY) {
        player->up = true;
    } else {
        player->up = false;
    }
    if (key_state & DOWN_KEY) {
        player->down = true;
    } else {
        player->down = false;
    }
    if (key_state & ATTACK_KEY) {
        player->shoot = true;
    } else {
        player->shoot = false;
    }
}
