#ifndef CLIENT_UDP_H
#define CLIENT_UDP_H

#ifdef __linux__
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#elif defined _WIN32 || defined _WIN64											// Added support for winsock
#include <WinSock2.h>															// Windows sockets
#include <ws2tcpip.h>															// getaddrinfo()
#endif
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//void cliSendTo(int sock, struct sockaddr_in srvAddr, int16_t id, int16_t keys);	// Send data from client to server over UDP
//int cliRecvfrom(int sock, int16_t *arrData);									// Receive data from server over UDP

#endif
