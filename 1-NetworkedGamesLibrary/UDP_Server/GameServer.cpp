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
#include "../Networking/NetJOR.h"
#include <sstream>										// 20180130 Updating text
//#include <string>	// to_string()

std::stringstream numPlayersText;

// get sockaddr, IPv4 or IPv6:
void *get_in_addr1(struct sockaddr *sa) {		 					// Already declared in Socket.h
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

bool GameServer::init() {
	numPlayers = 0;										// Start the game with 0 players (up to MAX_PLAYERS)
	parseType = -1;

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

	printColour("Server Running\nWaiting On Player Connections...", 9);					// Display text in blue font

	return true;
}

/*
	Send the number of players as a string to the connecting client, and use this as the ID for the client
*/
void GameServer::addPlayers() {
	char* playerConnect;

	//std::cout << "Waiting on player connections..." << std::endl;

	while (getNumPlayers() < getMaxNetPlayers()){								// While the number of players is less than the maximum number of players
		playerConnect = getInput();	
	
		std::cout << "playerConnect msg: " << playerConnect << std::endl;				// TEST: check the message received
	
		sscanf(playerConnect, "%d %s", &parseType, &(*parseString));

		if (parseType == 0) {
			std::cout << "New Player Connection" << std::endl;
			numPlayers++;
			std::cout << "Player " << getNumPlayers() << " is ready to start" << std::endl;

			numPlayersText.str("");
			numPlayersText << getNumPlayers() << " playerID";

			//std::cout << numPlayersText.str().c_str() << std::endl;
			
			//std::cout << "Test1" << std::endl;
			//NetJOR::Instance()->sendString(to_string(getNumPlayers()));				// Test new player connection on server
			// Need to send to client not server
			//NetJOR::Instance()->sendString(numPlayersText.str().c_str());				// Test new player connection on server
			addr_len = sizeof their_addr;
			sendto(sockfd, numPlayersText.str().c_str(), strlen(numPlayersText.str().c_str()), 0, (struct sockaddr *) &their_addr, addr_len);

			//std::cout << "Test2" << std::endl;
		}		
	}
	
	if (getNumPlayers() == getMaxNetPlayers())
		std::cout << "All Players Ready To Start" << std::endl;
}

void GameServer::update() {
	char* input;
/*	
	// Parse the recieved string buffer
	//sscanf(buf, "%d %s", &parseType, &(*parseString));							// Use integer value at start of string to decide how the received data is handled/parsed
*/
	input = getInput();///////////////////////////////////////////////////////////////////////////////

	int type = receivedMsgType(input);									// Check the type of message received from the player

	//parseCoordinates(input);										// 20180118 Parse the coordinates from the string buffer to integers
	if (type == 0) parseCoordinates(input);									// 20180118 Parse the coordinates from the string buffer to integers
	else if (type == 1) printColour("Player 1 Fired Laser", 12);
	else if (type == 2) printColour("Player 2 Fired Laser", 5);
	//if (!strcmp(buf, "exit")) break;									// exit the while loop
	else if (type == 3) {											// If the message type is 3
		printColour("Player 1 has quit the game", 12);							// Display exit message and
		//break;											// exit the while loop
	}
	else if (type == 4) printColour("Player 2 has quit the game", 5);
	else if (type == 5) printColour("Player 1 Laser Destroyed", 12);
	else if (type == 6) printColour("Player 2 Laser Destroyed", 5);

	// updateCoords();
	//updateClients();
}

void GameServer::updateClients() {
	printf("GameServer updateClients()");
	//sendto(sockfd, numPlayersText.str().c_str(), strlen(numPlayersText.str().c_str()), 0, (struct sockaddr *) &their_addr, addr_len);
	sendto(sockfd, "test", 5, 0, (struct sockaddr *) &their_addr, addr_len);
}

char* GameServer::getInput() {
	addr_len = sizeof their_addr;
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}

	//printf("%d bytes received from %s\n", numbytes, inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));	// Already declared in Socket.h
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
	//char name[20];
	int discard,id,x,y;

	// Type of message, name, x coordinate, y coordinate
	//sscanf(input, "%d %s %d %d", &discard, &(*name), &x, &y);	// Parse string data received from Game containing player name, and x and y coordinates
	sscanf(input, "%d %d %d %d", &discard, &id, &x, &y);		// Parse string data received from Game containing player name, and x and y coordinates
	//printCoords(name, x, y);					// Text.h
	printCoords(id, x, y);						// Text.h

	if (id == 1) {
		p1Addr = their_addr;
		// Send player 1 coords to player 2
		//addr_len = sizeof p2Addr;
		//sendto(sockfd, numPlayersText.str().c_str(), strlen(numPlayersText.str().c_str()), 0, (struct sockaddr *) &p2Addr, addr_len);
	} else if (id == 2) {
		p2Addr = their_addr;
		// Send player 2 coords to player 1
		//addr_len = sizeof p2Addr;
		//sendto(sockfd, numPlayersText.str().c_str(), strlen(numPlayersText.str().c_str()), 0, (struct sockaddr *) &p1Addr, addr_len);		
	}
}


