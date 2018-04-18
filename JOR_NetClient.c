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
#include <string.h>
#include "JOR_NetServer.h"																		// Server functionality
#include "JOR_NetText.h"																		// Debugging text

bool clientSocketReady = false;
int cliSock;
struct sockaddr_in cliAddr;																		// Client address structure
struct sockaddr_in getCliAddr() { return cliAddr; }

/*
	JOR_Net: Initialise the client socket
*/
bool JOR_NetInitClientUDP() {
	JOR_NetTextColour("\nJOR_Net: Initialising Client Socket\n\n", BLUE);

	JOR_NetTextColour("JOR_NetClientUDPSock: ", BLUE);											// Display label before error and info messages

    if ((cliSock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {										// Create UDP socket
		perror("Socket Failed");
		printf("Sock: %d\n", cliSock);
    } else {
		printf("\tClient Socket Created: %d\n", cliSock);
		clientSocketReady = true;
	}

	JOR_NetTextColour("JOR_NetClientUDPSock: ", BLUE);

    if (bind(cliSock, JN_SA &cliAddr, JN_SA_SZ) < 0) {											// Bind to address
		perror("Bind Error");
		printf("Sock: %d\n", cliSock);
    } else {
		printf("\tClient Bind OK: %d\n\n", cliSock);
		clientSocketReady = true;
	}

	return clientSocketReady;																	// The client is ready to communicate
}


/*
	JOR_Net: Create client sockaddr_in address structure
*/
struct sockaddr_in JOR_NetCliAddr() {
	memset(&cliAddr, 0, JN_SA_SZ);																// Initialise the address structure with 0s
	cliAddr.sin_family = AF_INET;																// Address family
	cliAddr.sin_addr.s_addr = INADDR_ANY;														// Socket accepts connections on all local IP addresses
	cliAddr.sin_port = 0;																		// Initialise port number
		
	return cliAddr;																				// Return the client address structure
}

/*
	JOR_Net: Send player data to the server
*/
void JOR_NetCliSendTo(struct sockaddr_in srvAddr, int16_t id, int16_t keys) {
	if (clientSocketReady) {
		int16_t arrData[2] = { id, keys };														// Client identifier, and key pressed

		//printf("cliSendTo arrdata0 %d arrdata1 %d\n", arrData[0], arrData[1]);

		if (sendto(cliSock, (char*)arrData, JN_I16_SZ * 2, 0, JN_SA &srvAddr, JN_SA_SZ) < 0) {	// Send data to 
			perror("Client sentToServer: sendto error");
		}
	}
}

/*
	JOR_Net: Recieve data from the server
*/
int JOR_NetCliRecvfrom(int16_t *arrData) {
	int numBytes = 0;

	if (clientSocketReady) {
		numBytes = recvfrom(cliSock, (char*)arrData, JN_I16_SZ * JN_BUF_MAX, 0, NULL, 0);		// Receive data from server
		//if (numBytes > 0) printf("Received from Server -  1: %d 2: %d 3: %d 4: %d\n", 
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

	JOR_NetTextColour("JOR_NetSetClientID: ", BLUE);
	printf("\tClient ID is now: %d\n", *clientID);												// Display the client ID
}

/*
	JOR_Net: Increase number of players if new player added
*/
bool JOR_NetCheckNewClient(int id, int *numPlayers) {
	if (id > *numPlayers) {																		// If the client id is higher than the number of players
		*numPlayers = id;																		// Set the number of players to match the ID

		JOR_NetTextColour("JOR_NetCheckNewClient: ", BLUE);
		printf("\nTotal players is now: %d\n", *numPlayers + 1);								// The number of players in the game has increased
		
		JOR_NetTextColour("New Connection: ", BLUE);											// Display new connection message in blue
		printf("\t%s\n", JOR_NetDisplayClientAddr(id));											// Display the client address

		return true;
	}

	return false;
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
