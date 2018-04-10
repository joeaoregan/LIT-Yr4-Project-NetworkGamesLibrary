/*
	Created By: Joe O'Regan
				K00203642

	Cross-platform UDP Network Games Library for C/C++

	JOR_NetServer.c

	Server Specific functions
*/

#include "stdafx.h"
#include "JOR_NetServer.h"
#include "Definitions.h"
#include "stdbool.h"																				// True, False
#include <string.h>																					// memset()
#include <stdio.h>																					// printf()	

struct sockaddr_in listOfClientAddresses[JN_MAX_PLAYERS];
int totalNumClients = 0;

/*
	JOR_Net: Return the list of connected client addresses
*/
struct sockaddr_in JOR_NetClientAddrList(int select) {
	return listOfClientAddresses[select];															// Client address list
}

/*
	JOR_Net: Initilise the server socket
*/
void JOR_NetInitServerUDP(int *sock, struct sockaddr_in *srvAddr) {
    memset(listOfClientAddresses, 0, sizeof(struct sockaddr_in) * JN_MAX_PLAYERS);					// Initialise the list of client addresses in memory
    if ((*sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {									// Initialise the server socket
        perror("JOR_NetInitServerUDP: Server_UDP createUDPServer: Socket Failed");
    }
	else printf("JOR_NetInitServerUDP: Server Socket Created: %d\n", (*sock));

    if (bind(*sock, (struct sockaddr*) srvAddr, sizeof(struct sockaddr)) < 0) {						// Bind the server socket
        perror("JOR_NetInitServerUDP: Bind Error");
    }
	else printf("JOR_NetInitServerUDP: Socket Bind OK\n\n");
}

/*
	JOR_Net: Send data from server to client ERROR WITH int16_t casting to char
*/
void srvSendto(int sock, struct sockaddr_in client, int16_t data[], int size) {
	socklen_t addr_size = sizeof(struct sockaddr);

	//sendto(sock, data, sizeof(int16_t) * size, 0, (struct sockaddr*)&client, addr_size);			// Send data to client
	sendto(sock, (char*) data, sizeof(int16_t) * size, 0, (struct sockaddr*)&client, addr_size);	// Send data to client
}

/*
	JOR_Net: Receive data from client
*/
struct sockaddr_in srvRecvfrom(int sock, int16_t data[]) {
	struct sockaddr_in addr;
	socklen_t addr_size = sizeof(struct sockaddr);
	//recvfrom(sock, data, sizeof(int16_t) * 4, 0, (struct sockaddr*)&addr, &addr_size);			// Receive data from client over UDP
	recvfrom(sock, (char*) data, sizeof(int16_t) * 4, 0, (struct sockaddr*)&addr, &addr_size);		// Receive data from client over UDP
	return addr;
}


/*
	JOR_Net: Add the client address to the list of connected clients
*/
void JOR_NetAddClientAddr(int clientNum, struct sockaddr_in *cliAddr) {
	//printf("server addClientAddrToList()\n");
    if (clientNum >= totalNumClients) {
        listOfClientAddresses[totalNumClients++] = *cliAddr;										// Add client address to the client address list
    }
}

/*
	JOR_Net: Check is the client already in the list of connected clients
*/
int JOR_NetExistingClient(int clientNum) {
	//printf("server existingClient() clientNum: %d totalNumClients: %d\n", clientNum, totalNumClients);
    return (clientNum < totalNumClients && clientNum >= 0);											// Is the client number is between 0 and the total number of clients
}

/*
	JOR_Net: Get the total number of clients
*/
int JOR_NetGetNumClients() { return totalNumClients; }												// Amount of connected clients


/*
	JOR_Net: Return the clients position in the list or the next position in the lists of clients for new client
*/
//int findClientIDNumber(struct sockaddr_in newCliAddr, struct sockaddr_in clientsList[], int numClients) {
int JOR_NetFindClientID(struct sockaddr_in newCliAddr, int numClients) {
    int i;
    for (i = 0; i < numClients; i++) {
        if (JOR_NetCompareAddr(&newCliAddr, &listOfClientAddresses[i])) {							// If the address matches and address in the existing clients
            return i;																				// Return clients position in the list									
        }
    }
    return numClients;																				// Return next client ID number
}

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
