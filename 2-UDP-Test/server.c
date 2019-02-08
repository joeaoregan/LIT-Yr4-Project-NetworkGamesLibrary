/*
	server.c

	Joe O'Regan
	16/02/2018

 	Server with bidirectional communication over UDP

	Default port 8888
*/

#include "socket.h"				// Includes, definitions, functions, and variables common to both client and server

int main(int argc, char *argv[]) {
	int sock, numbytes;			// Socket and number of bytes	
	struct sockaddr_in srvAddr, inAddr;	// Address structures
	char buf[BUFLEN];			// String buffer
	char obuf[BUFLEN];			// Out buffer
   
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) error("Creating socket");				// Create the socket

	bzero(&srvAddr,ADDR_SIZE);										// Clear the address, can also use memset()
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_addr.s_addr = INADDR_ANY;
	srvAddr.sin_port = htons((argc == 2) ? atoi(argv[1]) : UDP_PORT);					// If a port number is entered use that, otherwise use default
	if (bind(sock,(struct sockaddr *) &srvAddr, ADDR_SIZE) < 0) error("Binding to socket");

	while (1) {
		bzero(buf, BUFLEN);										// Clear the buffer

		numbytes = recvfrom(sock,buf,BUFLEN,0,(struct sockaddr *) &inAddr, &ADDR_SIZE);			// Recieve a message
		if (numbytes < 0) error("Server: recvfrom()");

		printf("Received From %s:%u: %s\n", inet_ntoa(inAddr.sin_addr), ntohs(inAddr.sin_port),buf);	// Display received message with client details
				
		sprintf(obuf, "Message Received: %s\n", buf);
		numbytes = sendto(sock, obuf, strlen(obuf), 0,(struct sockaddr *) &inAddr, ADDR_SIZE);		// Echo the message received back to the client
		if (numbytes < 0) error("Server: sendto()");
	}

	return 0;
 }

