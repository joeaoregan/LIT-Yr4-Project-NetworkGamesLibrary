/*
	Created By: Joe O'Regan
				K00203642

	Cross-platform UDP Network Games Library for C/C++

	JOR_NetServer.h

	Server Specific functions
*/

#pragma once


#if defined __linux__
#include <arpa/inet.h>
#elif defined _WIN32 || defined _WIN64

#ifdef JORNET_EXPORTS																		// Added by default to the defined preprocessor macros for the DLL project
#define JORNET_API __declspec(dllexport)													// Modifier set on the function declarations. Tells the compiler and linker to export a function or variable from the DLL so that it can be used by other applications
#else	
#define JORNET_API __declspec(dllimport)													// This modifier optimizes the import of the function or variable in an application
#endif

#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>																		// Windows sockets
#include <ws2tcpip.h>																		// getaddrinfo()fo()
#include <Windows.h>
#endif

#include <stdlib.h>																			// exit(), EXIT_FAILURE
#include <stdint.h>																			// int16_t
#include "Definitions.h"																	// BUF_MAX, JN_MODIFIER, MAX_PLAYERS
#include "stdbool.h"																		// Boolean values

void srvSendto(int sock, int clientID, int16_t tab[], int size);							// Use client ID to get address from list and send data to client using sendto()

// extern "C" guards are only required for C++
#ifdef __cplusplus
extern "C" {
#endif

	JN_MODIFIER char* JOR_NetDisplayClientAddr(int clientID);								// Print client address;

	JN_MODIFIER struct sockaddr_in JOR_NetServAddr(char *ip);								// JOR_Net: Create server sockaddr_in address structure

	JN_MODIFIER bool initServerUDPSock(int *sock, struct sockaddr_in *srvAddr);				// Create and bind the socket

	JN_MODIFIER bool JOR_NetInitServerListenUDP(struct sockaddr_in *srvAddr);				// Wrapper function for initialisng the socket

	JN_MODIFIER bool JOR_NetInitServerUDP(struct sockaddr_in *srvAddr);						// Create and bind the socket

	JN_MODIFIER void JOR_NetSrvSendto(int clientID, int16_t data[], int size);				// Send data to the client

	JN_MODIFIER void JOR_NetSrvSendID(int clientID, int16_t data[], int size);				// Send an ID to the client
	
	JN_MODIFIER int JOR_NetSrvRecvfrom(int16_t data[]);										// Receive data from client and return client ID
																		
	JN_MODIFIER void JOR_NetAddClientAddr(int client_pos, struct sockaddr_in *client_addr);	// Add the client address to the list of connected clients

	JN_MODIFIER int JOR_NetExistingClient(int client_pos);									// Check is the client already in the list of connected clients

	JN_MODIFIER int JOR_NetGetNumClients();													// Get the total number of clients

	JN_MODIFIER int JOR_NetFindClientID(struct sockaddr_in newCliAddr, unsigned int size);	// Find the client ID number

	JN_MODIFIER int JOR_NetCompareAddr(struct sockaddr_in *a, struct sockaddr_in *b);		// Compare to sockaddr_in addresses to see if there is a match

	JN_MODIFIER struct sockaddr_in JOR_NetGetClientAddr(int select);						// Use the client ID to get the client address and then send data

	JN_MODIFIER void JOR_NetCloseServerSocket();											// Close Server socket

#ifdef __cplusplus
}
#endif
