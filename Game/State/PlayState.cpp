/*
	Joe O'Regan
	K00203642
	
	PlayState.cpp

	Play the game
*/

#include "PlayState.h"
#include "../Texture.h"							// Texture functions
#include "../Player.h"							// Player functions
#include <list>								// Store game objects in a list
#include <sstream>							// 20180117 Updating text
#include "../Background.h"						// 20180121
#include "../Audio.h"							// 20180121
#include "../LaserManager.h"

#ifdef	__NETWORKING_JOE_O_REGAN					// Check for Windows version of game that Network Library is present, 20180123 Communicate with Server
#include "../../Networking/NetJOR.h"
#endif

#include "../Input.h"							// Handle input
#include "MainMenuState.h"

std::stringstream updateText;
std::vector<GameObject*> listOfGameObjects;				// List of game objects

GameObject* background;	 						// Scrolling background
GameObject* player;							// The Player that will be moving around on the screen	
GameObject* playerServer;						// The Player that will moved by coordinates received from the Server

// UI textures
Texture gTextTexture;

bool PlayState::onEnter() {
	std::cout << "Loading Play State" << std::endl;
	bool success = true;

	m_prevX = -1;
	m_prevY = -1;

	//success = Texture::Instance()->loadTextures();									// Load the game textures
	success = Texture::Instance()->loadTextures(LEVEL1_DATA);								// Load the LEVEL 1 textures
	if (success) std::cout << "Level Textures Loaded" << std::endl;

	success = Audio::Instance()->loadMediaAudio();										// Load audio asset files

	if( Game::Instance()->getFont() == NULL ) {
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	} else {
		//Render text
		SDL_Color textColor = { 255, 0, 0 };
		if( !gTextTexture.loadFromRenderedText( "Linux SDL Test Game - Joe O'Regan", textColor )) {
			printf( "Failed to render text texture!\n" );
			success = false;
		}
	}

	std::cout << "Font OK" << std::endl;

	// Create player and background, and add to game object list
	background = new Background();
	player = new Player();
	playerServer = new Player();
	player->setNetID(Game::Instance()->getAssignedNetID());									// Set the Players ID for network play

	std::cout << "PLAYER NET ID: " << Game::Instance()->getAssignedNetID() << std::endl;

	// TEST *** need to have this assigned elsewhere
	if(player->getNetID() == 1)												// Two player test
		playerServer->setNetID(2);											// Set the Server Players ID for network play
	else
		playerServer->setNetID(1);			

	std::cout << "PLAYER NET ID: " << Game::Instance()->getAssignedNetID() << std::endl;

	listOfGameObjects.push_back(background);
	listOfGameObjects.push_back(player);
	listOfGameObjects.push_back(playerServer);

	player->spawn(0,(SCREEN_HEIGHT - player->getHeight() - 120)/2, player->getVel());					// Center of play area
	playerServer->spawn(0,(SCREEN_HEIGHT - playerServer->getHeight() - 120)/2, playerServer->getVel());			// Center of play area
	std::cout << "Spawn Player" << std::endl;

	return success;
}

void PlayState::handleInput() {
	if (Input::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
		Game::Instance()->getStateMachine()->changeState(new MainMenuState());						// Open the main menu
}


void PlayState::update(){		
	for (int index = 0; index != listOfGameObjects.size(); ++index) {	
		listOfGameObjects[index]->update();										// Move/Update the game objects
	}

// SEND
	updateText.str("");													// 20180117			
	//updateText << "0 Player1 " << player->getX() << " " << player->getY();						// 20180118 Send name/ID, x coord, y coord  -  to server
	updateText << "0 " << player->getNetID() << " " << player->getX() << " " << player->getY();				// 20180118 Send name/ID, x coord, y coord  -  to server

	if (player->getX() != m_prevX || player->getY() != m_prevY) {								// Only send update if position changes
	#ifdef	__NETWORKING_JOE_O_REGAN											// Check for Windows version of game that Network Library is present
		NetJOR::Instance()->sendString(updateText.str().c_str());
	#endif
		m_prevX = player->getX();											// Set the new previous position
 		m_prevY = player->getY();
	}

	LaserManager::Instance()->update();											// Update lasers

// RECV
	//printf("PlayState update() recvfrom()");
	//char* input = NetJOR::Instance()->recvSrvMsg();	// recvfrom()
	//printf("test\n");
}

void PlayState::render() {
	for (int index = 0; index != listOfGameObjects.size(); ++index) {	
		listOfGameObjects[index]->render();										// Render the game object
	}

	gTextTexture.render((SCREEN_WIDTH - gTextTexture.getWidth())/2,((SCREEN_HEIGHT-600-gTextTexture.getHeight())/ 2)+600);

	LaserManager::Instance()->render();
}

bool PlayState::onExit() {
	std::cout << "Exit Play State" << std::endl;

	gTextTexture.free();

	return true;
}
