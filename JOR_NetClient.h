/*
	Created By: Joe O'Regan
				K00203642

	Cross-platform UDP Network Games Library for C/C++

	JOR_NetClient.h

	Client specific functions
*/

#pragma once

#ifdef JORNET_EXPORTS								// Added by default to the defined preprocessor macros for the DLL projet
#define JORNET_API __declspec(dllexport)			// Modifier set on the function declarations. Tells the compiler and linker to export a function or variable from the DLL so that it can be used by other applications
#else
#define JORNET_API __declspec(dllimport)			// This modifier optimizes the import of the function or variable in an application
#endif

#if defined __linux__
#include <arpa/inet.h>
#elif defined _WIN32 || defined _WIN64
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>																			// Windows sockets
#include <ws2tcpip.h>																			// getaddrinfo()
#endif
#include <stdio.h>																				// printf()
#include <stdint.h>																				// int16_t
#include "Definitions.h"																		// BUF_MAX, JN_MODIFIER

// extern "C" guards are only required when programming with C++
#ifdef __cplusplus
extern "C" {
#endif

JN_MODIFIER void JOR_NetClientUDPSock(int *sock, struct sockaddr_in *cliAddr);					// Create the client UDP socket

JN_MODIFIER void cliSendTo(int sock, struct sockaddr_in srvAddr, int16_t id, int16_t keys);		// Send data from client to server over UDP

JN_MODIFIER int cliRecvfrom(int sock, int16_t *arrData);										// Receive data from server over UDP

JN_MODIFIER void JOR_NetSetClientID(int id, int16_t* clientID, int *numPlayers);				// Set the ID for each player

JN_MODIFIER void JOR_NetCheckNewClient(int id, int *numPlayers);								// Increase number of players if new player added

#ifdef __cplusplus
}
#endif
