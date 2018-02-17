/*
	Joe O'Regan
	K00203642

	Socket.h
	16/01/2018
	
	UDP socket functions
	Cross platform functions to create sockets and send data to game server
*/
#ifndef	SOCKET_H
#define	SOCKET_H

// Unix (Ubuntu)
#if defined __linux__
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#elif defined _WIN32 || defined _WIN64
#include <WinSock2.h>							// Windows sockets
#include <ws2tcpip.h>							// getaddrinfo()
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#include "Error.h"

#define MAXBUFLEN 100
#define UDP_PORT "1066"							// The port the game connects to

//socklen_t ADDR_SIZE = sizeof(struct sockaddr_in);			// Length of sockaddr_in address structure, used for sending and receiving data

char buf[MAXBUFLEN];

void sendData(int fd, struct addrinfo *q, char* sendStr);

// Global
int sockfd;	// server socket
int servfd;
struct addrinfo *servinfo, *p;


// 20180130 Receive from server
socklen_t addr_len;
char server[INET6_ADDRSTRLEN];

void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// UDP Socket
bool createUDPSocket(char* serverName, char* msg){
	bool success = true;

	printf ("Connecting To Server: %s\n", serverName);

	int rv, numbytes;
	struct addrinfo hints;
/*
	if (argc == 1) {
		argv[1] = "localhost";
		argv[2] = "test";
	}
*/
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	//if ((rv = getaddrinfo(argv[1], UDP_PORT, &hints, &servinfo)) != 0) {
	if ((rv = getaddrinfo(serverName, UDP_PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		success = false;
		//return 1;
	}

	// Create a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("Game client: socket");
			success = false;
			continue;
		}
		break;
	}

	// Check socket
	if (p == NULL) {
		fprintf(stderr, "Game client: failed to bind socket\n");
		//return 2;
		success = false;
	}
/*
	if ((numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0, p->ai_addr, p->ai_addrlen)) == -1) {
		errorEx("talker: sendto", 1);
	}
*/
	//sendData(sockfd, p, "Socket.h test");
	//sendData(sockfd, p, msg);			// 20180130

	//freeaddrinfo(servinfo);

	//printf("Sent %d bytes to %s\n", numbytes, argv[1]);
	//close(sockfd);
	return success;
}

// Send data
void sendData(int fd, struct addrinfo *q, char* sendStr) {
	int byteCount;

	if ((byteCount = sendto(fd, sendStr, strlen(sendStr), 0, q->ai_addr, q->ai_addrlen)) == -1) {
		errorEx("sendData: sendto()", 1);
	}

	printf("Socket.h sendData() Sent %d bytes to %s\n", byteCount, sendStr);
}

void sendToServer(char* sendStr) {
	sendData(sockfd, p, sendStr);
}

// String stream requires const char* value
void sendToServer2(const char* sendStr) {
	sendData(sockfd, p, (char*) sendStr);
}

char* recvFromServer() {
	//char buf[MAXBUFLEN];
	//char* msg = "";
	int byteCount;
	struct sockaddr_storage servAddr;
	addr_len = sizeof servAddr;
	if ((byteCount = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&servAddr, &addr_len)) == -1) {
		errorEx("recvfrom()", 1);
	}
	//if ((byteCount = recvfrom(sockfd, msg, MAXBUFLEN-1 , 0, p->ai_addr, &p->ai_addrlen)) == -1) {
	//	errorEx("recvFromServer() recvfrom", 1);
	//}
	printf("%d bytes received from %s\n", byteCount, inet_ntop(servAddr.ss_family, get_in_addr((struct sockaddr *)&servAddr), server, sizeof server));
	//printf("%d bytes received from %s\n", byteCount, inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)&p), server, sizeof server));
	buf[byteCount] = '\0';
	printf("Socket.h recvFromServer() Message From Server: \"%s\"\n", buf);
	//msg = buf;
	//strcpy(msg, buf);
	//snprintf(msg, strlen(buf), buf);

	//printf("test");
	//std::cout << "msg: " << msg << std::endl;
	//printf("msg: %s\n", msg);

	return buf;						// Return the string received
}



// Moved from createUDPSocket(), call when game closes
void closeSocketStuff(){
	freeaddrinfo(servinfo);
	close(sockfd);
	printf("Socket.h: close socket, and free memory used to store address\n");
}

#endif	/* __SOCKET_H */
