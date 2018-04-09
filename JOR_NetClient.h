#pragma once

#ifdef JORNET_EXPORTS								// Added by default to the defined preprocessor macros for the DLL projet
#define JORNET_API __declspec(dllexport)			// Modifier set on the function declarations. Tells the compiler and linker to export a function or variable from the DLL so that it can be used by other applications
#else
#define JORNET_API __declspec(dllimport)			// This modifier optimizes the import of the function or variable in an application
#endif

#if defined __linux__
#include <netinet/in.h>
#include <arpa/inet.h>
#elif defined _WIN32 || defined _WIN64
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>																// Windows sockets
#include <ws2tcpip.h>																// getaddrinfo()
#endif
#include <string.h>
#include <stdio.h>
#include <stdlib.h>																	// exit()
#include <stdint.h>

#ifdef __cplusplus																	// extern "C" guards are only required when programming with C++
extern "C"
#endif
JORNET_API void JOR_NetClientUDPSock(int *sock, struct sockaddr_in *cliAddr);		// Create the client UDP socket

#ifdef __cplusplus
extern "C"
#endif
JORNET_API void JOR_NetSetClientID(int id, int16_t* clientID, int *numPlayers);		// Set the ID for each player

#ifdef __cplusplus																	
extern "C"
#endif
JORNET_API void JOR_NetCheckNewClient(int id, int *numPlayers);						// Increase number of players if new player added