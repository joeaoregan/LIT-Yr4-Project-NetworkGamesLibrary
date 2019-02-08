/*
	socket.h

	Joe O'Regan
	16/02/2018

	includes, definitions, functions, and variables common to both client and server
*/

#include <sys/types.h>
#include <stdlib.h>					// exit()
#include <unistd.h>					// close()
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>					// memset()
//#include <sys/time.h> 				// select()

#include <arpa/inet.h>					// client only

#define SERV_ADDR "127.0.0.1"				// Default Server address
#define UDP_PORT 8888					// Default Port number

#define BUFLEN 512					// Length of the buffer
//unsigned int ADDR_SIZE = sizeof(struct sockaddr_in);	// Length of sockaddr_in address structure, used for sending and receiving data
socklen_t ADDR_SIZE = sizeof(struct sockaddr_in);	// Length of sockaddr_in address structure, used for sending and receiving data

// display error message
void error(const char *msg) {
    perror(msg);
    exit(0);
}

// Display error message, and specify exit condition
void errorEx(const char *msg, int type) {
    perror(msg);
    exit(type);
}
