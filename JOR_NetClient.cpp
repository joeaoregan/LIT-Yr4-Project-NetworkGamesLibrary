#if defined __linux__
#include <netinet/in.h>
#include <arpa/inet.h>
#elif defined _WIN32 || defined _WIN64
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>																// Windows sockets
#include <ws2tcpip.h>																// getaddrinfo()
#endif
#include <string.h>
#include <stdio.h>
#include <stdlib.h>																	// exit()
#include <stdint.h>

#include "stdafx.h"
#include "JOR_NetClient.h"

/*
	JOR_Net: Initialise the client socket
*/
void JOR_NetClientUDPSock(int *sock, struct sockaddr_in *cliAddr) {
    if ((*sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {								// Create UDP socket
		perror("JOR_NetClientUDPSock: Socket Failed");
    }
	else printf("JOR_NetClientUDPSock: Client Socket Created: %d\n", (*sock));

    if (bind(*sock, (struct sockaddr*) cliAddr, sizeof(struct sockaddr)) < 0) {		// Bind to address
		perror("JOR_NetClientUDPSock: Bind Error");
    }
	else printf("JOR_NetClientUDPSock: Client Bind OK\n\n");
}

/*
	JOR_Net: Set the ID for each player
*/
void JOR_NetSetClientID(int id, int16_t* clientID, int *numPlayers) {
	*clientID = id;																	// Set the client id
	*numPlayers = id;																// Number of players in the game
	printf("JOR_Net setClientID(): Client ID is now: %d\n", *clientID);
}

/*
	JOR_Net: Increase number of players if new player added
*/
void JOR_NetCheckNewClient(int id, int *numPlayers) {
	if (id > *numPlayers) {															// If the client id is higher than the number of players
		*numPlayers = id;															// Set the number of players to match the ID
		printf("JOR_NetCheckNewClient: Total players is now: %d\n", *numPlayers + 1);// The number of players in the game has increased
	}
}