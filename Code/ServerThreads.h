/*
	Modified by:	Joe O'Regan
					K00203642

	ServerThreads.h

	Modified server functionality to work with windows
	Then moved Network functionality to JOR_Net
*/

#ifndef SERVER_UDP_H
#define SERVER_UDP_H

#if defined __linux__
#include <arpa/inet.h>
#endif

int serverInputLoop(void *arg);													// JOR Changed return type to int		
int serverOutputLoop(void *arg);												// JOR Changed return type to int

#endif
