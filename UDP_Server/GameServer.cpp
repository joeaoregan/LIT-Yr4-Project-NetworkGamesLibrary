/*
	Joe O'Regan
	K00203642

	GameServer.cpp
	30/01/2018

	Change over from C to C++
*/
#include "GameServer.h"
#include "Text.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <iostream>
#include <string.h>

bool GameServer::init() {
	numPlayers = 0;										// Start the game with 0 players (up to MAX_PLAYERS)

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

	return true;
}

void GameServer::addPlayers() {
	char* playerConnect;

	std::cout << "Waitin on player connections..." << std::endl;

	while (getNumPlayers() < getMaxNetPlayers()){	// While the number of players is less than the maximum number of players
		//numPlayers++;
		//std::cout << "Player " << getNumPlayers() << " has connected" << std::endl;

		playerConnect = getInput();		
		sscanf(playerConnect, "%d %s", &parseType, &(*parseString));
		if (parseType == 0) {
			std::cout << "New Player Connection" << std::endl;
			numPlayers++;
			std::cout << "Player " << getNumPlayers() << " is ready to start" << std::endl;
		}
	}
	
	if (getNumPlayers() == getMaxNetPlayers())
		std::cout << "Ready to start" << std::endl;
}

void GameServer::update() {
	char* input;
/*
	addr_len = sizeof their_addr;
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}

	//printf("%d bytes received from %s\n", numbytes, inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));
	buf[numbytes] = '\0';
	
	// Parse the recieved string buffer
	//sscanf(buf, "%d %s", &parseType, &(*parseString));							// Use integer value at start of string to decide how the received data is handled/parsed
*/
	input = getInput();

	int type = receivedMsgType(input);									// Check the type of message received from the player

	//parseCoordinates(input);										// 20180118 Parse the coordinates from the string buffer to integers
	if (type == 0)
		parseCoordinates(input);									// 20180118 Parse the coordinates from the string buffer to integers
	else if (type == 1)
		printColour("Player Fired Laser", 12);

	//if (!strcmp(buf, "exit")) break;									// exit the while loop
	else if (type == 3) {											// If the message type is 3
		printColour("Client has terminated connection", 5);						// Display exit message and
		//break;											// exit the while loop
	}
	else if (type == 4){
		printColour("Laser Desroyed", 12);
	}
}

char* GameServer::getInput() {
	addr_len = sizeof their_addr;
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}

	//printf("%d bytes received from %s\n", numbytes, inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));
	buf[numbytes] = '\0';
	
	return buf;	
}

void GameServer::close_gs() {
#if defined __linux__
	close(sockfd);
#endif
}

int GameServer::receivedMsgType(char* input) {
	sscanf(input, "%d %s", &parseType, &(*parseString));

	return parseType;
}

void GameServer::parseCoordinates(char* input) {
	char name[20];
	int discard,x,y;

	// Type of message, name, x coordinate, y coordinate
	sscanf(input, "%d %s %d %d", &discard, &(*name), &x, &y);		// Parse string data received from Game containing player name, and x and y coordinates
	printCoords(name, x, y);
}

/*
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
*/
