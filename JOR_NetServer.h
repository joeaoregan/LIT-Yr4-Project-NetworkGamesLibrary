/*
	Created By: Joe O'Regan
				K00203642

	UDP Network Games Library for C/C++

	JOR_NetServer.h

	Server Specific functions
*/

#pragma once

#ifdef JORNET_EXPORTS																			// Added by default to the defined preprocessor macros for the DLL projet
#define JORNET_API __declspec(dllexport)														// Modifier set on the function declarations. Tells the compiler and linker to export a function or variable from the DLL so that it can be used by other applications
#else	
#define JORNET_API __declspec(dllimport)														// This modifier optimizes the import of the function or variable in an application
#endif

#if defined __linux__
#include <netinet/in.h>
#include <arpa/inet.h>
#elif defined _WIN32 || defined _WIN64
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>																			// Windows sockets
#include <ws2tcpip.h>																			// getaddrinfo()fo()
#include <Windows.h>
#endif
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>																				// exit()
#include <stdint.h>
				
#include "Definitions.h"																		// MAX_PLAYERS definition

// extern "C" guards are only required when programming with C++
#ifdef __cplusplus
extern "C" {
#endif

extern JORNET_API void JOR_NetInitServerUDP(int *sock, struct sockaddr_in *server_sock);		// Create and bind the socket
																		
extern JORNET_API void srvSendto(int sock, struct sockaddr_in client, int16_t tab[], int size);	// Use sendto() to send data to client
																		
extern JORNET_API struct sockaddr_in srvRecvfrom(int sock, int16_t data[]);						// Receive data from client
																		
extern JORNET_API void JOR_NetAddClientAddr(int client_pos, struct sockaddr_in *client_addr);	// Add the client address to the list of connected clients

extern JORNET_API int JOR_NetExistingClient(int client_pos);									// Check is the client already in the list of connected clients

extern JORNET_API int JOR_NetGetNumClients();													// Get the total number of clients

extern JORNET_API int JOR_NetFindClientID(struct sockaddr_in newCliAddr, int size);				// Find the client ID number

extern JORNET_API int JOR_NetCompareAddr(struct sockaddr_in *a, struct sockaddr_in *b);			// Compare to sockaddr_in addresses to see if there is a match

extern JORNET_API struct sockaddr_in JOR_NetClientAddrList(int select);							// Returns the list of client addresses


#ifdef __cplusplus
}
#endif