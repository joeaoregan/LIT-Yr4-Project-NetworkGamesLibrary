/*
	Created By: Joe O'Regan
				K00203642

	UDP Network Games Library for C/C++

	JOR_NetClient.cpp

	Client specific functions
*/

#if defined __linux__
#include <unistd.h>																				// close(), usleep()
#endif
#include "stdafx.h"																				// Visual Studio file  (can't wrap)
#include "JOR_NetClient.h"

bool clientSocketReady = false;
int cliSock;

/*
	JOR_Net: Initialise the client socket
*/
bool JOR_NetClientUDPSock(struct sockaddr_in *cliAddr) {
	printf("JOR_Net: Initialising Client Socket\n");

    if ((cliSock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {										// Create UDP socket
		perror("JOR_NetClientUDPSock: Socket Failed");
		printf("Sock: %d\n", cliSock);
    } else {
		printf("JOR_NetClientUDPSock: Client Socket Created: %d\n", cliSock);
		clientSocketReady = true;
	}

    if (bind(cliSock, JN_SA cliAddr, JN_SA_SZ) < 0) {											// Bind to address
		perror("JOR_NetClientUDPSock: Bind Error");
		printf("Sock: %d\n", cliSock);
    } else {
		printf("JOR_NetClientUDPSock: Client Bind OK\n\n");
		clientSocketReady = true;
	}

	return clientSocketReady;
}

/*
	Send player data to the server
*/
void cliSendTo(struct sockaddr_in srvAddr, int16_t id, int16_t keys) {
	if (clientSocketReady) {
		int16_t arrData[2] = { id, keys };														// Client identifier, and key pressed

		//printf("cliSendTo arrdata0 %d arrdata1 %d\n", arrData[0], arrData[1]);

		if (sendto(cliSock, (char*)arrData, JN_I16_SZ * 2, 0, JN_SA &srvAddr, JN_SA_SZ) < 0) {
			perror("Client sentToServer: sendto error");
		}
	}
}

/*
	Recieve data from the server
*/
int cliRecvfrom(int16_t *arrData) {
	int numBytes = 0;

	if (clientSocketReady) {
		numBytes = recvfrom(cliSock, (char*)arrData, JN_I16_SZ * JN_BUF_MAX, 0, NULL, 0);		// Receive data from server
		//printf("Received from Server -  1: %d 2: %d 3: %d 4: %d\n", 
		//	arrData[0], arrData[1], arrData[2], arrData[3]);									// Display data received
	}

    return numBytes;																			// Return the number of bytes received
}

/*
	JOR_Net: Set the ID for each player
*/
void JOR_NetSetClientID(int id, int16_t* clientID, int *numPlayers) {
	*clientID = id;																				// Set the client id
	*numPlayers = id;																			// Number of players in the game
	printf("JOR_Net setClientID(): Client ID is now: %d\n", *clientID);
}

/*
	JOR_Net: Increase number of players if new player added
*/
void JOR_NetCheckNewClient(int id, int *numPlayers) {
	if (id > *numPlayers) {																		// If the client id is higher than the number of players
		*numPlayers = id;																		// Set the number of players to match the ID
		printf("JOR_NetCheckNewClient: Total players is now: %d\n", *numPlayers + 1);			// The number of players in the game has increased
	}
}

/*
	JOR_Net: Close Client Socket
*/
void JOR_NetCloseClientSocket() {
#if defined __linux__
	close(cliSock);																				// Close client socket
#elif defined _WIN32 || defined _WIN64
	closesocket(cliSock);																		// Close the client socket
	WSACleanup();																				// Terminate use of Winsock 2 DLL
#endif
}
