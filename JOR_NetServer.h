/*
	Created By: Joe O'Regan
				K00203642

	Cross-platform UDP Network Games Library for C/C++

	JOR_NetServer.h

	Server Specific functions
*/

#pragma once

#ifdef JORNET_EXPORTS																		// Added by default to the defined preprocessor macros for the DLL project
#define JORNET_API __declspec(dllexport)													// Modifier set on the function declarations. Tells the compiler and linker to export a function or variable from the DLL so that it can be used by other applications
#else	
#define JORNET_API __declspec(dllimport)													// This modifier optimizes the import of the function or variable in an application
#endif

#if defined __linux__
#include <arpa/inet.h>
#elif defined _WIN32 || defined _WIN64
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>																		// Windows sockets
#include <ws2tcpip.h>																		// getaddrinfo()fo()
#include <Windows.h>
#endif

#include <stdlib.h>																			// exit(), EXIT_FAILURE
#include <stdint.h>																			// int16_t
#include "Definitions.h"																	// BUF_MAX, JN_MODIFIER, MAX_PLAYERS
#include "stdbool.h"

// extern "C" guards are only required when programming with C++
#ifdef __cplusplus
extern "C" {
#endif

	//JN_MODIFIER void JOR_NetInitServerUDP(int *sock, struct sockaddr_in *server_sock);	// Create and bind the socket
	JN_MODIFIER bool JOR_NetInitServerUDP(struct sockaddr_in *server_sock);					// Create and bind the socket

	//JN_MODIFIER void srvSendto(int sock, struct sockaddr_in client, int16_t tab[], int size);	// Use sendto() to send data to client
	JN_MODIFIER void srvSendto(struct sockaddr_in client, int16_t tab[], int size);			// Use sendto() to send data to client

	//JN_MODIFIER struct sockaddr_in srvRecvfrom(int sock, int16_t data[]);					// Receive data from client
	JN_MODIFIER struct sockaddr_in srvRecvfrom(int16_t data[]);								// Receive data from client
																		
	JN_MODIFIER void JOR_NetAddClientAddr(int client_pos, struct sockaddr_in *client_addr);	// Add the client address to the list of connected clients

	JN_MODIFIER int JOR_NetExistingClient(int client_pos);									// Check is the client already in the list of connected clients

	JN_MODIFIER int JOR_NetGetNumClients();													// Get the total number of clients

	JN_MODIFIER int JOR_NetFindClientID(struct sockaddr_in newCliAddr, int size);			// Find the client ID number

	JN_MODIFIER int JOR_NetCompareAddr(struct sockaddr_in *a, struct sockaddr_in *b);		// Compare to sockaddr_in addresses to see if there is a match

	JN_MODIFIER struct sockaddr_in JOR_NetClientAddrList(int select);						// Returns the list of client addresses

	JN_MODIFIER void JOR_NetCloseServerSocket();											// Close Server socket

#ifdef __cplusplus
}
#endif
