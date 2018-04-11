/*
	Created By: Joe O'Regan
				K00203642

	Cross-platform UDP Network Games Library for C/C++

	JOR_Net.h
	
	Main header file, used to access the library from within a game/app
	Functionality that isn't specific to Client or Server
*/

#pragma once

#ifdef JORNET_EXPORTS											// Added by default to the defined preprocessor macros for the DLL projet
#define JORNET_API __declspec(dllexport)						// Modifier set on the function declarations. Tells the compiler and linker to export a function or variable from the DLL so that it can be used by other applications
#else
#define JORNET_API __declspec(dllimport)						// This modifier optimizes the import of the function or variable in an application
#endif

#if defined _WIN32 || defined _WIN64
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>											// Windows sockets
#include <ws2tcpip.h>											// getaddrinfo()
#endif
#include "JOR_NetClient.h"										// Client functionality
#include "JOR_NetServer.h"										// Server functionality

// extern "C" guards only required when programming with C++
#ifdef __cplusplus
extern "C" {
#endif

	JN_MODIFIER struct sockaddr_in JOR_NetServAddr(char *ip);	// JOR_Net: Create server sockaddr_in address structure
	
	JN_MODIFIER void JOR_NetInitWinsock();						// JOR_Net: Init windows networking functionality

	JN_MODIFIER void JOR_NetCloseSocket();						// JOR_Net: Init windows networking functionality

	JN_MODIFIER void JOR_NetSleep(int amount);					// JOR_Net: Function to handle sleep for both Windows and Linux

#ifdef __cplusplus
}
#endif

