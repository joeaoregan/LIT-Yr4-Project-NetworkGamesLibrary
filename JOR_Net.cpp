// JOR_Net.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "JOR_Net.h"


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
