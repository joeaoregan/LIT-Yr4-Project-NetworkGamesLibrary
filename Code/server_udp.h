/*
	Modified server functionality to work with windows
*/

#ifndef SERVER_UDP_H
#define SERVER_UDP_H

#if defined __linux__
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#elif defined _WIN32 || defined _WIN64
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>															// Windows sockets
#include <ws2tcpip.h>															// getaddrinfo()
#endif
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void createUDPServer(int *sock, struct sockaddr_in *server_sock);				// Create and bind the socket
void srvSendto(int sock, struct sockaddr_in client, int16_t tab[], int size);	// Use sendto() to send data to client
int serverInputLoop(void *arg);													// JOR Changed return type to int		
int serverOutputLoop(void *arg);												// JOR Changed return type to int
int existingClient(int client_pos);												// Check is the client already in the list of connected clients
void addClientAddrToList(int client_pos, struct sockaddr_in *client_addr);		// Add the client address to the list of connected clients

#endif
