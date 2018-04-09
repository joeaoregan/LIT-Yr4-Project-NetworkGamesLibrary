/*
	Created By: Joe O'Regan
				K00203642
*/

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
#include <WinSock2.h>												// Windows sockets
#include <ws2tcpip.h>												// getaddrinfo()
#endif
#include <string.h>
#include <stdio.h>
#include <stdlib.h>													// exit()
#include <stdint.h>
#include "JOR_NetClient.h"											// Client functionality
#include "JOR_NetServer.h"											// Server functionality
#include "Definitions.h"


#ifdef __cplusplus													// extern "C" guards are only required when programming with C++
extern "C"
#endif
JORNET_API struct sockaddr_in JOR_NetServAddr(char *ip);			// JOR_Net: Create server sockaddr_in address structure

#ifdef __cplusplus
extern "C"
#endif
JORNET_API struct sockaddr_in JOR_NetCliAddr();						// JOR_Net: Create client sockaddr_in address structure

#ifdef __cplusplus
extern "C" 
#endif
JORNET_API void JOR_NetInitWinsock();								// JOR_Net: Init windows networking functionality


