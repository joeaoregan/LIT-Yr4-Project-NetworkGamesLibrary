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

#if defined __linux__
#include <unistd.h>																		// close(), usleep()
#endif
#include "stdafx.h"
#include "JOR_Net.h"
#include <string.h>																		// memset()
#include "Definitions.h"																// JN_SERV_ADDR, JN_SERV_PORT
#include "JOR_NetText.h"

/*
	JOR_Net: Init windows networking functionality
*/
void JOR_NetInitWinsock() {
#if defined _WIN32 || defined _WIN64
	JOR_NetTextColour("JOR_NetInitWinsock: ", GOLD);
	printf(": Initialised Winsock.\n");													// Otherwise indicate winsock has initialised
	WSADATA wsa;																		// Windows sockets implementation structure
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {										// If winsock doesn't initialise
		printf("JOR_NetInitWinsock: FAILED!!! Error Code : %d", WSAGetLastError());		// Display an error message
		exit(EXIT_FAILURE);																// And exit with the specified error code
	}

#endif
}

/*
	JOR_Net: Close the sockets
*/
void JOR_NetCloseSocket() {
	JOR_NetCloseClientSocket();															// Close client socket
	JOR_NetCloseServerSocket();															// Close server socket

#if defined _WIN32 || defined _WIN64
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
	Sleep(amount/1000);																	// Windows value measured in milliseconds??? Was dividing by 1000
#endif
}

/*
	JOR_Net: Display Logo
*/
void JOR_NetInit() {
	JOR_NetTextColour("     ____.________ __________      _______          __\n", RED);
	JOR_NetTextColour("    |    |\\_____  \\\\______   \\     \\      \\   _____/  |_\n", RED);
	JOR_NetTextColour("    |    | /   |   \\|       _/     /   |   \\_/ __ \\   __\\\n", RED);
	JOR_NetTextColour("/\\__|    |/    |    \\    |   \\    /    |    \\  ___/|  |\n", RED);
	JOR_NetTextColour("\\________|\\_______  /____|_  /____\\____|__  /\\___  >__|\n", RED);
	JOR_NetTextColour("                  \\/       \\/_____/       \\/     \\/\n", RED);
	printf("Cross-Platform UDP Networked Games Library by ");
	JOR_NetTextColour("Joe O'Regan\n\n", BLUE);

}
