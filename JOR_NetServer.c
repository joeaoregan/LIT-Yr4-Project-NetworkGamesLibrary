/*
	Created By: Joe O'Regan
				K00203642

	Cross-platform UDP Network Games Library for C/C++

	JOR_NetServer.c

	Server Specific functions
*/

#if defined __linux__
#include <unistd.h>																					// close(), usleep()
#endif
#include "stdafx.h"
#include "JOR_NetServer.h"
#include "Definitions.h"
#include "stdbool.h"																				// True, False
#include <string.h>																					// memset()
#include <stdio.h>																					// printf()	
#include "JOR_NetText.h"

bool serverSocketReady = false;
bool getServSockReady() { return serverSocketReady; }
struct sockaddr_in listOfClientAddresses[JN_MAX_PLAYERS];											// Connected client addresses
unsigned int totalNumClients = 0;																	// Number of connected clients
int srvSock, srvSockListen;																			// Server Socket, additional listening socket for server needed to listen on any local address
char* cliAddrStr[INET6_ADDRSTRLEN];																	// Char string to hold human readable address
char str[INET6_ADDRSTRLEN];

/*
char* JOR_NetDisplayClientAddr(int clientID) {
	struct sockaddr_in cliAddr =  JOR_NetGetClientAddr(clientID);
																// Char string to hold human readable address
	inet_ntop(AF_INET, &(cliAddr.sin_addr), cliAddrStr, INET6_ADDRSTRLEN);									// Get the IP address
	snprintf(str, INET6_ADDRSTRLEN + 5, "%s:%d\n", cliAddrStr, ntohs(cliAddr.sin_port));

	return str;
}
*/

/*
	JOR_Net: Return the list of connected client addresses
*/
struct sockaddr_in JOR_NetGetClientAddr(int select) {
	return listOfClientAddresses[select];															// Client address list
}

/*
	JOR_Net: Initilise the server socket
*/
bool JOR_NetInitServerListenUDP(struct sockaddr_in *srvAddr) {
	JOR_NetTextColour("\nInitialise Server Listen Socket\n\n", RED);
	serverSocketReady = initServerUDPSock(&srvSockListen, srvAddr);

	return serverSocketReady;
}
/*
	JOR_Net: Initilise the server socket
*/
bool JOR_NetInitServerUDP(struct sockaddr_in *srvAddr) {
	/*
    memset(listOfClientAddresses, 0, JN_SA_SZ * JN_MAX_PLAYERS);									// Initialise the list of client addresses in memory

    if ((srvSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {									// Initialise the server socket
        perror("JOR_NetInitServerUDP: Server_UDP createUDPServer: Socket Failed");
		printf("JOR_NetInitServerUDP: Socket: %d\n", srvSock);
    } else {
		printf("JOR_NetInitServerUDP: Server Socket Created: %d\n", (srvSock));
		serverSocketReady = true;
	}

    if (bind(srvSock, (struct sockaddr*) srvAddr, JN_SA_SZ) < 0) {									// Bind the server socket
        perror("JOR_NetInitServerUDP: Bind Error");
		printf("Sock: %d\n", srvSock);
    } else {
		printf("JOR_NetInitServerUDP: Socket Bind OK\n\n");
		serverSocketReady = true;
	}
	*/
	JOR_NetTextColour("\nInitialise Server Send Socket\n\n", RED);

	serverSocketReady = initServerUDPSock(&srvSock, srvAddr);

	return serverSocketReady;
}

bool initServerUDPSock(int *sock, struct sockaddr_in *srvAddr) {
	// set SO_REUSEADDR on a socket to true (1):
	//int optval = 1;
	//setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

	memset(listOfClientAddresses, 0, JN_SA_SZ * JN_MAX_PLAYERS);

	if ((*sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("JOR_NetInitServerUDP: Server_UDP createUDPServer: Socket Failed");
		JOR_NetTextColour("JOR_NetInitServerUDP: ", RED);
		printf("Socket Error: %d\n", *sock);
	} else {
		JOR_NetTextColour("JOR_NetInitServerUDP: ", RED);
		 printf("Server Socket Created: %d\n", (*sock));
		 serverSocketReady = true;
	 }
		
	if (bind(*sock, (struct sockaddr*) srvAddr, JN_SA_SZ) < 0) {									// Bind the server socket
		JOR_NetTextColour("JOR_NetInitServerUDP: ", RED);
		perror("Bind Error");
		printf("Sock: %d\n", *sock);
	}
	else {
		JOR_NetTextColour("JOR_NetInitServerUDP: ", RED);
		printf("Socket Bind OK: %d\n", *sock);
		serverSocketReady = true;
	}

	return serverSocketReady;
}


/*
	JOR_Net: Create server sockaddr_in address structure
*/
struct sockaddr_in JOR_NetServAddr(char *ip) {
    struct sockaddr_in srvAddr;															// Server address structure
    memset(&srvAddr, 0, sizeof(srvAddr));												// Initialise address
    srvAddr.sin_family = AF_INET;														// Address family socket can communicate with

	// If the IP address was not entered from a client set to INADDR_ANY
    if (ip == NULL) {																	// If the IP address is not already set
#if defined __linux__
		srvAddr.sin_addr.s_addr = INADDR_ANY;											// Linux Server: listen on any local address
#elif defined _WIN32 || defined _WIN64
		inet_pton(AF_INET, JN_SERV_ADDR, &srvAddr.sin_addr);							// Windows Server: Specify the address as 127.0.0.1
#endif
    } else {
		inet_pton(AF_INET, ip, &srvAddr.sin_addr);										// JOR Replace inet_aton with inet_pton
    }

    srvAddr.sin_port = htons(JN_SERV_PORT);												// Set the port number to communicate on

	char str[INET6_ADDRSTRLEN];															// Char string to hold human readable address
	inet_ntop(AF_INET, &(srvAddr.sin_addr), str, INET6_ADDRSTRLEN);						// Get the IP address
	JOR_NetTextColour("JOR_NetServAddr: ", RED);
	printf("Server Address: %s:%d\n", str, ntohs(srvAddr.sin_port));	// Display the IP address

    return srvAddr;
}


void JOR_NetSrvSendto(int clientID, int16_t data[], int size) {
	srvSendto(srvSock, clientID, data, size);
}
void JOR_NetSrvSendID(int clientID, int16_t data[], int size) {
	srvSendto(srvSockListen, clientID, data, size);
}

/*
	JOR_Net: Send data from server to client ERROR WITH int16_t casting to char
*/
void srvSendto(int sock, int clientID, int16_t data[], int size) {
	struct sockaddr_in client = JOR_NetGetClientAddr(clientID);										// Get the client address using its ID

	if (serverSocketReady) {
		socklen_t addr_size = JN_SA_SZ;

		sendto(sock, (char*)data, JN_I16_SZ * size, 0, (struct sockaddr*)&client, addr_size);	// Send data to client
	}
}

/*
	JOR_Net: Receive data from client
*/
int srvRecvfrom(int16_t arrData[]) {
	struct sockaddr_in cliAddr;
	socklen_t addr_size = JN_SA_SZ;
	int curClient;

	recvfrom(srvSock, (char*) arrData, JN_I16_SZ * 4, 0, (struct sockaddr*) &cliAddr, &addr_size);	// Receive data from client over UDP

	curClient = JOR_NetFindClientID(cliAddr, totalNumClients);

	if (arrData[0] == -1 && curClient < JN_MAX_PLAYERS) {											// ID field of the data array is -1, the client is new, and still under max players
		JOR_NetAddClientAddr(curClient, &cliAddr);													// Add the client address to the list of connected clients
	}

	return curClient;
}

/*
	JOR_Net: Return the clients position in the list or the 
	next position in the lists of clients for new client
*/
int JOR_NetFindClientID(struct sockaddr_in newCliAddr, unsigned int numClients) {
    unsigned int i;
    for (i = 0; i < numClients; i++) {
        if (JOR_NetCompareAddr(&newCliAddr, &listOfClientAddresses[i])) {							// If the address matches and address in the existing clients
            return i;																				// Return clients position in the list									
        }
    }
    return numClients;																				// Return next client ID number
}

/*
	JOR_Net: Add the client address to the list of connected clients
*/
void JOR_NetAddClientAddr(int clientNum, struct sockaddr_in *cliAddr) {
	//printf("server addClientAddrToList()\n");
    if (clientNum >= (int) totalNumClients) {
        listOfClientAddresses[totalNumClients++] = *cliAddr;										// Add client address to the client address list
    }
}

/*
	JOR_Net: Check is the client already in the list of connected clients
*/
int JOR_NetExistingClient(int clientNum) {
	//printf("server existingClient() clientNum: %d totalNumClients: %d\n",
	//clientNum,totalNumClients);
    return (clientNum < (int) totalNumClients && clientNum >= 0);									// Is the client number is between 0 and the total number of clients
}

/*
	JOR_Net: Get the total number of clients
*/
int JOR_NetGetNumClients() { return totalNumClients; }												// Amount of connected clients

/*
	JOR_Net: Compare to sockaddr_in addresses to see if there is a match
*/
int JOR_NetCompareAddr(struct sockaddr_in *a, struct sockaddr_in *b) {
	if (a->sin_port == b->sin_port &&																// If the port number, 
		a->sin_family == b->sin_family &&															// Address family, 
		a->sin_addr.s_addr == b->sin_addr.s_addr) {													// and address match
		return true;																				// Indicate they are a matching pair of addresses
	}

	return false;																					// Otherwise no match found
}

/*
	JOR_Net: Close Server Socket
*/
void JOR_NetCloseServerSocket() {
#if defined __linux__
	close(srvSock);																					// Close Server socket
#elif defined _WIN32 || defined _WIN64
	closesocket(srvSock);																			// Close the Server socket
	WSACleanup();																					// Terminate use of Winsock 2 DLL
#endif
}
