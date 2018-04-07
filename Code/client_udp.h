#ifndef CLIENT_UDP_H
#define CLIENT_UDP_H

#ifdef __linux__
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#elif defined _WIN32 || defined _WIN64
#include <WinSock2.h>							// Windows sockets
#include <ws2tcpip.h>							// getaddrinfo()
#endif
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void initClientUDPSock(int *sock, struct sockaddr_in *cliAddr);
void cliSendTo(int sock, struct sockaddr_in srvAddr, int16_t id, int16_t keys);
int cliRecvfrom(int sock, int16_t *arrData);

#endif
