/* 	
	client.c

	Joe O'Regan
	16/02/2018

	UDP client sends and receives messages from server
*/

#include "socket.h"								// Includes, definitions, functions, and variables common to both client and server

int main(int argc, char *argv[]) {
	int sock, numbytes;							// Socket, number of bytes sent/received
	struct sockaddr_in srvAddr, inAddr;					// Address structures
	struct hostent *hp;
	char buf[BUFLEN];							// String buffer

	if (argc < 1 || argc > 3) { 
		errorEx("Usage: server port [Optional]\n", 1);
	}

	if ((sock= socket(AF_INET, SOCK_DGRAM, 0)) < 0) error("socket");	// Create the socket

	srvAddr.sin_family = AF_INET;
	hp = gethostbyname((argc >= 2) ? argv[1] : SERV_ADDR);			// If an IP address is entered (parameter 2) use that, otherwise use default value
	if (hp == 0) error("Unidentified host");

	bcopy((char *)hp->h_addr, (char *)&srvAddr.sin_addr, hp->h_length);
	srvAddr.sin_port = htons((argc == 3) ? atoi(argv[2]) : UDP_PORT);	// If a port number is entered (parameter 3) use that, otherwise use default value

	printf("Enter Message: ");
	bzero(buf, BUFLEN);							// Clear the buffer
	fgets(buf, BUFLEN-1, stdin);						// Get input from keyboard

	if ((numbytes = sendto(sock, buf, strlen(buf),0,(const struct sockaddr *) &srvAddr, ADDR_SIZE)) < 0)
		error("sendto()");

	if ((numbytes = recvfrom(sock,buf,BUFLEN,0,(struct sockaddr *) &inAddr, &ADDR_SIZE)) < 0)
		error("recvfrom()");

	printf("Server Response: %s", buf);

	close(sock);
	return 0;
}

