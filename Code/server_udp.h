#ifndef SERVER_DUP_H
#define SERVER_DUP_H

#if defined __linux__
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#elif defined _WIN32 || defined _WIN64
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>							// Windows sockets
#include <ws2tcpip.h>							// getaddrinfo()
#endif
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void prepare_server(int *sock, struct sockaddr_in *server_sock);
void send_data(int sock, struct sockaddr_in client, int16_t tab[], int size);
void* server_receive_loop(void *arg);
void* server_send_loop(void *arg);
//int server_receive_loop(void *arg);												// Changed return type to int		
//int server_send_loop(void *arg);												// Changed return type to int
int its_an_old_client(int client_pos);
void add_adr_to_list(int client_pos, struct sockaddr_in *client_addr);

#endif
