/*
	Joe O'Regan
	K00203642
	
	Server.c
	17/01/2018

	UDP Server socket to read data from Game Client
*/
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

#define UDP_PORT "1066" 									// Port number to connect to
#define MAXBUFLEN 100

// Colours
#define RED   "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELO  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define MGNT  "\x1B[35m"
#define CYAN  "\x1B[36m"
#define NORM  "\x1B[0m"

void parseCoordinates(char* input);
 
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void) {
	int sockfd, rv, numbytes, parseType;			// 20180118 Decide what function is used to parse the string received from the game client
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN], parseString[MAXBUFLEN];		// 20180118 Added parse string, to separate received data from game client
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

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
			close(sockfd);
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

	printf("Waiting on data...\n");

	// Keep receiving until "exit" received // 20180118 Replaced with message type
	while(1) {
		addr_len = sizeof their_addr;
		if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
			perror("recvfrom");
			exit(1);
		}
		//printf("%d bytes received from %s\n", numbytes, inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));
		buf[numbytes] = '\0';
		//printf("Message Recieved: \"%s\"\n", buf);

		// Parse the recieved string buffer
		sscanf(buf, "%d %s", &parseType, &(*parseString));							// Use integer value at start of string to decide how the received data is handled/parsed

		//parseCoordinates(buf);										// 20180118 Parse the coordinates from the string buffer to integers
		if (parseType == 0)
			parseCoordinates(buf);										// 20180118 Parse the coordinates from the string buffer to integers
		else if (parseType == 1)
			printf("%sPlayer Fired Laser%s\n",RED,NORM);

		//if (!strcmp(buf, "exit")) break;									// exit the while loop
		else if (parseType == 3) {											// If the message type is 3
			printf("%sClient has terminated connection%s\n",MGNT,NORM);					// Display exit message and
			//break;												// exit the while loop
		}
		else if (parseType == 4){
			printf("%sLaser Desroyed%s\n", RED,NORM);
		}
	}
	close(sockfd);

	return 0;
}

//void parseCoordinates(char* input, int x, int y) {
void parseCoordinates(char* input) {
	//printf("parseCoordinates()");

	char name[20];
	int discard,x,y;

	sscanf(input, "%d %s %d %d", &discard, &(*name), &x, &y);							// Parse string data received from Game containing player name, and x and y coordinates
	//printf("X: %d\n", x);

	printf("%sPlayer:%s\t%s\t%sX:%s\t%d%s\tY:%s\t%d\n", BLUE, NORM, name, BLUE, NORM, x, BLUE, NORM, y);		// Display Coordinates

	//if (lives > 0) printf("%sWord To Guess:%s\t\t%s\n", CYAN, NORM, name);					// The part word string while game is playing OR
	//else printf("%sThe word was:%s\t\t%s\n", RED, NORM, name);							// The actual word if player loses
}

