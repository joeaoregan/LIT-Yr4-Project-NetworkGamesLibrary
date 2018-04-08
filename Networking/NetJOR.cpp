/*
	Joe O'Regan
	K00203642

	NetJOR.h

	Main networking file
	Using singleton pattern to access variables and functionality
*/

// Unix (Ubuntu)
#if defined __linux__
#include <SDL2/SDL.h>								// SDL Linux
// Windows
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>								// SDL Windows
#endif

#include "NetJOR.h"
#include "Socket.h"								// createUDPSocket()
#include <iostream>

NetJOR* NetJOR::s_pInstance = 0;						// Networking singleton

Uint32 lastTime;


bool NetJOR::init(){
	bool success;

	std::cout << "Init Server" << std::endl;
	
	success = createUDPSocket("localhost", "socket test" );			// Socket.h - *** CHANGE TO BOOLEAN for return type

	return success;
}

void NetJOR::update(){
	// Update server?
/*
	if (SDL_GetTicks() >= lastTime + 1000) {				// Every second
		lastTime = SDL_GetTicks();					// Reset the time
		std::cout << "NetJOR update(): Count 1 second" << std::endl;	// Will update stuff at time intervals later
	}
*/
}

void NetJOR::render(){
	// Display networking information in game window?
}

//void NetJOR::close(){
void NetJOR::close(int id){
	// Identify the player that is leaving the game
	if (id == 1)
		sendToServer("3 exit");						// Let the server know player 1 has left the game
	else if (id == 2)
		sendToServer("4 exit");						// Let the server know player 2 has left the game

	closeSocketStuff();
}

void NetJOR::handleEvents(){
	// Add keyboard button press events here later
}

void NetJOR::sendString(const char* sendStr) {
	sendData(sockfd, p, (char*) sendStr);
}
/*
void NetJOR::sendText(char* sendStr) {				// Only using one function to send data
	sendData(sockfd, p, sendStr);
}
*/
void NetJOR::sendKeyPress() {
	
}

char* msg;	// needs to be outside function

/*
	MainMenuState connectToServer()
*/
char* NetJOR::recvSrvMsg() {
	//char msg[100] = recvFromServer();
	msg = recvFromServer();
	std::cout << "NetJOR recvSrvMsg(): " << msg << std::endl;

	return msg;
}

void NetJOR::receiveNetID(){
	std::cout << "Receive net id from server" << std::endl;
}
