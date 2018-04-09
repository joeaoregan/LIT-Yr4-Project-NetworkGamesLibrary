/*
	Created By: Joe O'Regan
				K00203642

	UDP Network Games Library for C/C++

	JOR_NetClient.cpp

	Client specific functions
*/

#if defined __linux__
#include <netinet/in.h>
#include <arpa/inet.h>
#elif defined _WIN32 || defined _WIN64
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>																	// Windows sockets
#include <ws2tcpip.h>																	// getaddrinfo()
#endif
#include <string.h>
#include <stdio.h>
#include <stdlib.h>																		// exit()
#include <stdint.h>

#include "stdafx.h"
#include "JOR_NetClient.h"
#include "Definitions.h"																// BUF_MAX

/*
	JOR_Net: Initialise the client socket
*/
void JOR_NetClientUDPSock(int *sock, struct sockaddr_in *cliAddr) {
    if ((*sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {									// Create UDP socket
		perror("JOR_NetClientUDPSock: Socket Failed");
    }
	else printf("JOR_NetClientUDPSock: Client Socket Created: %d\n", (*sock));

    if (bind(*sock, (struct sockaddr*) cliAddr, sizeof(struct sockaddr)) < 0) {			// Bind to address
		perror("JOR_NetClientUDPSock: Bind Error");
    }
	else printf("JOR_NetClientUDPSock: Client Bind OK\n\n");
}

/*
	Send player data to the server
*/
void cliSendTo(int sock, struct sockaddr_in srvAddr, int16_t id, int16_t keys) {
	int16_t arrData[2] = { id, keys };													// Client identifier, and key pressed
	//if (sendto(sock, arrData, sizeof(int16_t) * 2, 0, (struct sockaddr *) &srvAddr, sizeof(struct sockaddr)) < 0) {
	if (sendto(sock, (char*) arrData, sizeof(int16_t) * 2, 0, (struct sockaddr *) &srvAddr, sizeof(struct sockaddr)) < 0) {
		perror("Client sentToServer: sendto error");
    } 
}

/*
	Recieve data from the server
*/
int cliRecvfrom(int sock, int16_t *arrData) {
	//int numBytes = recvfrom(sock, arrData, sizeof(int16_t) * BUF_MAX, 0, NULL, 0);
	int numBytes = recvfrom(sock, (char*) arrData, sizeof(int16_t) * JN_BUF_MAX, 0, NULL, 0);	// Receive data from server
	//printf("Received from Server -  1: %d 2: %d 3: %d 4: %d\n", 
	//	arrData[0], arrData[1], arrData[2], arrData[3]);								// Display data received
    return numBytes;																	// Return the number of bytes received
}

/*
	JOR_Net: Set the ID for each player
*/
void JOR_NetSetClientID(int id, int16_t* clientID, int *numPlayers) {
	*clientID = id;																		// Set the client id
	*numPlayers = id;																	// Number of players in the game
	printf("JOR_Net setClientID(): Client ID is now: %d\n", *clientID);
}

/*
	JOR_Net: Increase number of players if new player added
*/
void JOR_NetCheckNewClient(int id, int *numPlayers) {
	if (id > *numPlayers) {																// If the client id is higher than the number of players
		*numPlayers = id;																// Set the number of players to match the ID
		printf("JOR_NetCheckNewClient: Total players is now: %d\n", *numPlayers + 1);	// The number of players in the game has increased
	}
}