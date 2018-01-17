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

#define UDP_PORT "1066" 					// the port users will be connecting to

void sendData(int fd, struct addrinfo *q, char* sendStr);
void sendToServer(char* sendStr); 

// Global
int sockfd;
struct addrinfo *p;

int main(int argc, char *argv[]) {
	//int sockfd, rv, numbytes;
	int rv, numbytes;
	//struct addrinfo hints, *servinfo, *p;
	struct addrinfo hints, *servinfo;

	if (argc == 1) {
		argv[1] = "localhost";
		argv[2] = "test";
	}
	/*
	if (argc != 3) {
		fprintf(stderr,"usage: talker hostname message\n");
		exit(1);
	}
	*/
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(argv[1], UDP_PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// Create a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}
		break;
	}

	// Check socket
	if (p == NULL) {
		fprintf(stderr, "talker: failed to bind socket\n");
		return 2;
	}
/*
	if ((numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0, p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}
*/
	freeaddrinfo(servinfo);

	//printf("Sent %d bytes to %s\n", numbytes, argv[1]);

	
	sendData(sockfd, p, "test");

	close(sockfd);

	return 0;
}

void sendData(int fd, struct addrinfo *q, char* sendStr) {
	int byteCount;
	//char* sendStr = "test";

	if ((byteCount = sendto(fd, sendStr, strlen(sendStr), 0, q->ai_addr, q->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}

	printf("Sent %d bytes to %s\n", byteCount, sendStr);
}


void sendToServer(char* sendStr) {
	sendData(sockfd, p, sendStr);
}
