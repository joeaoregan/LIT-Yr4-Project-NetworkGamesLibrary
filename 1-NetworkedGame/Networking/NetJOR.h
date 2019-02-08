/*
	Joe O'Regan
	K00203642

	NetJOR.h

	Main networking header file
	Using singleton pattern to access variables and functionality
*/

#ifndef	__NETWORKING_JOE_O_REGAN
#define	__NETWORKING_JOE_O_REGAN

//#include "../Game.h"
//#include "Socket.h"

class NetJOR {
public:
	// Networking Singleton
	static NetJOR* Instance() {
		if (s_pInstance == 0) {					// If there is no instance of Game
			s_pInstance = new NetJOR();			// Create new game instance
			return s_pInstance;					// and return it
		}

		return s_pInstance;						// Return the current game instance
	}

	bool init();								// Initialise network stuff

	void update();								// Update the networking
	void render();								// Draw the networking to screen

	void close(int id);							// Close socket stuff, 20180208 identify which player has left game

	void handleEvents();

	void netDestroyGameObject();				// Notify server an object has been destroyed

	// String stream requires const char* value
	void sendString(const char* sendStr);		// Sent the string
	//void sendText(char* sendStr);				// Send text

	void sendKeyPress();						// 20180403 Send key press to server

	void receiveNetID(); 

	char* recvSrvMsg();

private:

	static NetJOR* s_pInstance;					// Networking singleton instance
};

#endif	/* __NETWORKING_JOE_O_REGAN */
