/*
	Created By: Joe O'Regan
				K00203642

	Cross-platform UDP Network Games Library for C/C++

	Definitions.h
*/

#ifndef JOR_NET_DEFINITIONS_H
#define JOR_NET_DEFINITIONS_H

#if defined __linux__
#include <arpa/inet.h>
#endif

// Modifier for functions used in Visual studio for importing / exporting functions to 
#if defined __linux__
#define JN_MODIFIER extern						// Modifier not required for Linux
#elif defined _WIN32 || defined _WIN64
#define JN_MODIFIER extern JORNET_API			// Use modifier
#endif

#define JN_MAX_PLAYERS 10						// JOR_Net: Maximum 10 connected players
#define JN_SERV_ADDR "127.0.0.1"  				// JOR IP address of udp server (localhost)
#define JN_SERV_PORT 8887						// JOR Server port number
#define JN_BUF_MAX 256							// JOR_Net: Max buffer size
#define JN_SA (struct sockaddr *)				// JOR_Net: Cast to struct sockaddr*
#define JN_SA_SZ sizeof(struct sockaddr)		// JOR_Net: Size of a sockaddr structure
#define JN_I16_SZ sizeof(int16_t)				// JOR_Net: Size of int16_t type

#endif
