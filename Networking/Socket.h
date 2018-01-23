/*
	Joe O'Regan
	K00203642

	Socket.h
	16/01/2018
	
	UDP socket functions
	Create sockets and send data
*/
#ifndef	SOCKET_H
#define	SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define UDP_PORT "1066"							// The port the game connects to

void sendData(int fd, struct addrinfo *q, char* sendStr);

// Global
int sockfd;
struct addrinfo *servinfo, *p;

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
		perror("talker: sendto");
		exit(1);
	}
*/
	//sendData(sockfd, p, "Socket.h test");
	sendData(sockfd, p, msg);

	//freeaddrinfo(servinfo);

	//printf("Sent %d bytes to %s\n", numbytes, argv[1]);
	//close(sockfd);
	return success;
}

// Send data
void sendData(int fd, struct addrinfo *q, char* sendStr) {
	int byteCount;

	if ((byteCount = sendto(fd, sendStr, strlen(sendStr), 0, q->ai_addr, q->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}

	printf("Sent %d bytes to %s\n", byteCount, sendStr);
}

void sendToServer(char* sendStr) {
	sendData(sockfd, p, sendStr);
}

// String stream requires const char* value
void sendToServer2(const char* sendStr) {
	sendData(sockfd, p, (char*) sendStr);
}

// Moved from createUDPSocket(), call when game closes
void closeSocketStuff(){
	freeaddrinfo(servinfo);
	close(sockfd);
	printf("Socket.h: close socket, and free memory used to store address\n");
}

#endif	/* __SOCKET_H */
