/*
	Modified by:	Joe O'Regan
					K00203642

	Originally by:	Maciej Spychala

	ServerThreads.h

	Modified server functionality to work with windows
	Then moved Network functionality to JOR_Net
*/

#ifndef SERVER_THREAD_H
#define SERVER_THREAD_H

#if defined __linux__
#include <arpa/inet.h>
#endif

int serverInputLoop(void *arg);		// JOR Changed return type to int to comply with SDL_Thread	
int serverOutputLoop(void *arg);	// JOR Changed return type to int

#endif
