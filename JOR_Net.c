/*
	Created By: Joe O'Regan
				K00203642

	Cross-platform UDP Network Games Library for C/C++

	JOR_Net.c

	Main header file, used to access the library from within a game/app
	Functionality that isn't specific to Client or Server

	Changed from C++ to C
*/

// JOR_Net.c : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "JOR_Net.h"

#if defined __linux__
#include <unistd.h>																		// close()
#endif
#include <string.h>																		// memset()
#include "Definitions.h"																// JN_SERV_ADDR, JN_SERV_PORT

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
		inet_pton(AF_INET, JN_SERV_ADDR, &srvAddr.sin_addr);							// Windows Server: Specify the address as 127.0.0.1
#endif
    } else {
		inet_pton(AF_INET, ip, &srvAddr.sin_addr);										// JOR Replace inet_aton with inet_pton
    }

    srvAddr.sin_port = htons(JN_SERV_PORT);												// Set the port number to communicate on

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

/*
	JOR_Net: Close the sockets
*/
void JOR_NetCloseSocket(int cli, int srv) {
#if defined __linux__
	close(cli);																			// Close client
	close(srv);																			// Close server
#elif defined _WIN32 || defined _WIN64
	closesocket(cli);																	// Close the client socket
	closesocket(srv);																	// Close the server socket
	WSACleanup();																		// Terminate use of Winsock 2 DLL
#endif
}

/*
	JOR_Net: Function to handle sleep for both Windows and Linux
	Fixes problems with microseconds/milliseconds
*/
void JOR_NetSleep(int amount) {
#if defined __linux__
	usleep(amount);																		// Linux value measured in microseconds
#elif defined _WIN32 || defined _WIN64
	Sleep(amount / 1000);																// Windows value measured in milliseconds
#endif
}
