/*
	Joe O'Regan
	K00203642
	
	Server.c
	17/01/2018

	UDP Server socket to read data from Game Client
*/
// Linux
#if defined __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
// Windows
#elif defined _WIN32 || defined _WIN64
#include <WinSock2.h>										// Windows sockets
#include <ws2tcpip.h>										// getaddrinfo()
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include "Text.h"

#define UDP_PORT "1066" 									// Port number to connect to
#define MAXBUFLEN 100

void parseCoordinates(char* input);
 
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void) {
	int sockfd, rv, numbytes, parseType;							// 20180118 Decide what function is used to parse the string received from the game client
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN], parseString[MAXBUFLEN];						// 20180118 Added parse string, to separate received data from game client
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; 								// set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; 								// use my IP

	if ((rv = getaddrinfo(NULL, UDP_PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
#if defined __linux__
			close(sockfd);
#endif
			perror("listener: bind");
			continue;
		}
		break;
	}

	if (p == NULL) {
		fprintf(stderr, "Socket bind failed\n");
		return 2;
	}

	freeaddrinfo(servinfo);

	printColour("Server Running\nWaiting on connections...", 9);

	// Make the connection

	// Keep receiving until "exit" received // 20180118 Replaced with message type
	while(1) {
		addr_len = sizeof their_addr;
		if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
			perror("recvfrom");
			exit(1);
		}
		//printf("%d bytes received from %s\n", numbytes, inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));
		buf[numbytes] = '\0';

		// Parse the recieved string buffer
		sscanf(buf, "%d %s", &parseType, &(*parseString));							// Use integer value at start of string to decide how the received data is handled/parsed

		//parseCoordinates(buf);										// 20180118 Parse the coordinates from the string buffer to integers
		if (parseType == 0)
			parseCoordinates(buf);										// 20180118 Parse the coordinates from the string buffer to integers
		else if (parseType == 1)
			printColour("Player Fired Laser", 12);

		//if (!strcmp(buf, "exit")) break;									// exit the while loop
		else if (parseType == 3) {										// If the message type is 3
			printColour("Client has terminated connection", 5);						// Display exit message and
			//break;											// exit the while loop
		}
		else if (parseType == 4){
			printColour("Laser Desroyed", 12);
		}
	}
#if defined __linux__
	close(sockfd);
#endif

	return 0;
}

void parseCoordinates(char* input) {
	char name[20];
	int discard,x,y;

	sscanf(input, "%d %s %d %d", &discard, &(*name), &x, &y);							// Parse string data received from Game containing player name, and x and y coordinates
	printCoords(name, x, y);
}

