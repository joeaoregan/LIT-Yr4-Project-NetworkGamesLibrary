#include "client_udp.h"
#include "Definitions.h"

//unsigned int sequenceNumber = 0; // track messages with sequence number

/*
	Initialise the client socket
*/
void createClientUDPSock(int *sock, struct sockaddr_in *cliAddr) {
    if ((*sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Client_UDP createClientUDPSock: Socket Failed");
    }
	else printf("Client Socket Created: %d\n", (*sock));

    if (bind(*sock, (struct sockaddr*) cliAddr, sizeof(struct sockaddr)) < 0) {
		perror("Client_UDP createClientUDPSock: Bind Error");
    }
	else printf("Client Bind OK\n\n");
}

/*
	Send player data to the server
*/
void cliSendTo(int sock, struct sockaddr_in srvAddr, int16_t id, int16_t keys) {
	int16_t arrData[2] = { id, keys };																				// Client identifier, and key pressed
    if (sendto(sock, arrData, sizeof(int16_t) * 2, 0, (struct sockaddr *) &srvAddr, sizeof(struct sockaddr)) < 0) {	
		perror("Client sentToServer: sendto error");
    } 
}

/*
	Recieve data from the server
*/
int cliRecvfrom(int sock, int16_t *arrData) {
    int numBytes = recvfrom(sock, arrData, sizeof(int16_t) * BUF_MAX, 0, NULL, 0);
	//printf("Received from Server -  1: %d 2: %d 3: %d 4: %d\n", arrData[0], arrData[1], arrData[2], arrData[3]);	// Display data received
    return numBytes;																								// Return the number of bytes received
}

/*
	Set the ID for each player
*/
void setClientID(int id, int16_t* clientID, int *numPlayers) {
	*clientID = (int16_t) id;																						// Set the client id
	*numPlayers = (int16_t) id;																						// Number of players in the game
	printf("main.c->setClientID: Client ID is now: %d\n", *clientID);
}

/*
	Increase number of players if new player added
*/
void checkIfNewPlayer(int id, int *numPlayers) {
	if (id > *numPlayers) {																							// If the client id is higher than the number of players
		*numPlayers = id;																							// Set the number of players to match the ID
		printf("Total number of players is now: %d\n", *numPlayers + 1);											// The number of players in the game has increased
	}
}
