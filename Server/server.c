/*
	Joe O'Regan K00203642
	server.c
	09/12/2017
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <syslog.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>								// write(), read(), close()

extern time_t time ();

# define MAXLEN 80 /* Maximum size in the world of Any string */
# define TCP_PORT 1066

void  get_coords(int in, int out);

int main () {
	int sock, fd, client_len;
 	struct sockaddr_in server, client;
 	char* guess[MAXLEN];
	char* outbuf [MAXLEN];

 	srand ((int) time ((long *) 0)); /* randomize the seed */

 	sock = socket (AF_INET, SOCK_STREAM, 0);//0 or IPPROTO_TCP
 	if (sock <0) { //This error checking is the code Stevens wraps in his Socket Function etc
 		perror ("creating stream socket");
 		exit (1);
 	}

 	server.sin_family = AF_INET;
 	server.sin_addr.s_addr = htonl(INADDR_ANY);
 	server.sin_port = htons(TCP_PORT);

 	if (bind(sock, (struct sockaddr *) & server, sizeof(server)) <0) {
 		perror ("binding socket");
	 	exit (2);
 	}

 	listen (sock, 5);
	printf("Server\n");

 	while (1) {
 		client_len = sizeof (client);
 		//if ((fd = accept (sock, (struct sockaddr *) &client, &client_len)) <0) {
 		//	perror ("accepting connection");
 		//	exit (3);
 		//}

 		accept (sock, (struct sockaddr *) &client, &client_len);

		printf("loop\n");
		while(1){
			//write(sock,"test\n", 5);
			recv(fd, guess, MAXLEN, 0);
			
		write(1,guess, 80);
		}

		printf("loop2\n");
 		//get_coords(fd, fd);

 		close (fd);
 	}

	return 0;
 }

void  get_coords(int in, int out) {
 	char guess[MAXLEN], outbuf [MAXLEN];
 	char hostname[MAXLEN];

	printf("get_coords\n");

 	//gethostname (hostname, MAXLEN);

 	//while (read(in, guess, MAXLEN) > 0) {
 	while (1) {
		//read(in, guess, MAXLEN);
		recv(in, guess, MAXLEN,0);
		write(1,guess, 80);

	 	//sprintf (outbuf, "%s\n", guess);
	 	//write (out, outbuf, strlen (outbuf));
 	}
 }
