/*
	Joe O'Regan
	K00203642

	GameServer.h
	30/01/2018

	GameServer header file
*/
#ifndef GAME_SERVER_H
#define GAME_SERVER_H

// Linux
#if defined __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
// Windows
#elif defined _WIN32 || defined _WIN64
#include <WinSock2.h>					// Windows sockets
#include <ws2tcpip.h>					// getaddrinfo()
#endif

#define UDP_PORT "1066" 				// Port number to connect to
#define MAXBUFLEN 100
#define MAX_PLAYERS 2					// Start with 2 players and work my way up

class GameServer {
public:
	bool init();
	void addPlayers();				// *** WILL NEED SOME KIND OF GAME LOBBY FOR ADDING PLAYERS AT START
	void update();
	void close_gs();

	char* getInput();				// Get input from connected clients (recvfrom())
	void updateClients();				// Send updated coordinates

	void parseCoordinates(char* input);
	int receivedMsgType(char* input);
	
	// get/set methods
	int getMaxNetPlayers() { return MAX_PLAYERS; }
	int getNumPlayers() { return numPlayers; }

private:
	// socket stuff;
	int sockfd, rv, numbytes;			// 20180118 Decide what function is used to parse the string received from the game client
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr, p1Addr, p2Addr;
	char buf[MAXBUFLEN], parseString[MAXBUFLEN];	// 20180118 Added parse string, to separate received data from game client
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];

	// Received game data stuff
	int playerID;					// 1. Identify the player data received belongs to
	int parseType;					// 2. Type of message received

	// Send game data stuff
	unsigned int numPlayers;			// The number of players in the game
};
#endif
