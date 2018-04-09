// JOR_Net.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "JOR_Net.h"

/*
	JOR_Net: Create server sockaddr_in address structure
*/
struct sockaddr_in JOR_NetServAddr(char *ip) {
    struct sockaddr_in srvAddr;															// Server address structure
    memset(&srvAddr, 0, sizeof(srvAddr));												// Initialise address
    srvAddr.sin_family = AF_INET;														// Address family socket can communicate with

	// If the IP address was not entered from a client set to INADDR_ANY
    if (ip == NULL) {																	// If the IP address is not already set
		//srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
#if defined __linux__
		srvAddr.sin_addr.s_addr = INADDR_ANY;											// Linux Server: listen on any local address
#elif defined _WIN32 || defined _WIN64
		inet_pton(AF_INET, SERV_ADDR, &srvAddr.sin_addr);								// Windows Server: Specify the address as 127.0.0.1
#endif
    } else {
		inet_pton(AF_INET, ip, &srvAddr.sin_addr);										// JOR Replace inet_aton with inet_pton
    }

    srvAddr.sin_port = htons(SERV_PORT);												// Set the port number to communicate on

	char str[INET6_ADDRSTRLEN];															// Char string to hold human readable address
	inet_ntop(AF_INET, &(srvAddr.sin_addr), str, INET6_ADDRSTRLEN);						// Get the IP address
	printf("JOR_NetServAddr: Server Address: %s:%d\n", str, ntohs(srvAddr.sin_port));	// Display the IP address

    return srvAddr;
}

/*
	JOR_Net: Create client sockaddr_in address structure
*/
struct sockaddr_in JOR_NetCliAddr() {
	struct sockaddr_in cliAddr;															// Client address structure
	memset(&cliAddr, 0, sizeof(struct sockaddr));										// Initialise the address structure
	cliAddr.sin_family = AF_INET;														// Address family
	cliAddr.sin_addr.s_addr = INADDR_ANY;												// Socket accepts connections on all local IP addresses
	cliAddr.sin_port = 0;																// Initialise port number
		
	return cliAddr;
}

/*
	JOR_Net: Init windows networking functionality
*/
void JOR_NetInitWinsock() {
#if defined _WIN32 || defined _WIN64
	WSADATA wsa;																		// Windows sockets implementation structure
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {										// If winsock doesn't initialise
		printf("JOR_NetInitWinsock: FAILED!!! Error Code : %d", WSAGetLastError());		// Display an error message
		exit(EXIT_FAILURE);																// And exit with the specified error code
	}

	printf("JOR_NetInitWinsock: Initialised Winsock.\n");								// Otherwise indicate winsock has initialised
#endif
}
