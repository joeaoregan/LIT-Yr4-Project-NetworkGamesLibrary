
#include "MainMenuState.h"
#include "../MenuButton.h"
#include "../Texture.h"
#include "../Background.h"
#include "PlayState.h"
#include "../MenuButton.h"
#include "../Input.h"

#include "../../Networking/ServerUDP/Text.h"

#ifdef	__NETWORKING_JOE_O_REGAN					// Check for Windows version of game that Network Library is present, 20180123 Communicate with Server
#include "../../Networking/NetJOR.h"
#endif

//Texture bg;
//GameObject* menuBackground;
GameObject* menuButton1;
GameObject* menuButton2;
GameObject* menuButton3;

Texture playerNetIDTexture;

bool MainMenuState::onEnter() {
	//std::cout << "Loading Menu State" << std::endl;
	printColour("Loading Menu State", 12);

	bool success = true;

	success = Audio::Instance()->LoadFX();

	numButtons = 3;											// int in MenuState.h
	currentBtn = 1;
	setButtonPressed();										// Disable ability to press button, and time before button can be pressed again
	

	success = Texture::Instance()->loadTextures(MENU_DATA);						// Load the menu textures
	if (success) std::cout << "Menu Textures Loaded" << std::endl;

	//menuBackground = new Background();
	//menuBackground->setTextureID("logoID");
	//listOfMenuObjects.push_back(menuBackground);

	menuButton1 = new MenuButton(1);
	menuButton2 = new MenuButton(2);
	menuButton3 = new MenuButton(3);
	menuButton1->setTextureID("playBtnID");
	menuButton2->setTextureID("connectBtnID");
	menuButton3->setTextureID("readyBtnID");
	
	// Position buttons
	menuButton1->setX(440);
	menuButton1->setY(0);
	menuButton2->setX(440);
	menuButton2->setY(100);
	menuButton3->setX(440);
	menuButton3->setY(200);


	m_gameObjects.push_back(menuButton1);
	m_gameObjects.push_back(menuButton2);
	m_gameObjects.push_back(menuButton3);

	// Callbacks need to be set after adding game objects to list for MainMenuState::setCallbacks()
	m_callbacks.push_back(0);
	m_callbacks.push_back(startGame);								// Add the game state to callbacks list
	m_callbacks.push_back(connectToServer);								// Add connect to server function
	m_callbacks.push_back(readyToStart);								// Add ready to start function

	setCallbacks(m_callbacks);									// Set the callbacks for menu items

	m_loadingComplete = true;

	highlightCurrentButton(&m_gameObjects);								// Highlight the current button

	printColour("Menu State: Loading Complete", 12);

	// Indicate player netID
	SDL_Color textColor = { 255, 0, 0 };
	if( !playerNetIDTexture.loadFromRenderedText( "Player has not yet been assigned a Net ID", textColor )) {
		printf( "Failed to render player net id texture!\n" );
		success = false;
	}

	return success;
}

void MainMenuState::handleInput() {
	for (int index = 0; index != m_gameObjects.size(); ++index) {	
		dynamic_cast<MenuButton*>(m_gameObjects[index])->handleEvents((Input::Instance()->getEvent()), index);		// Update the input
	}
}

void MainMenuState::update(){	
	if (!buttonPressed()) {
		MenuState::update();									// Up and down buttons

		if (Input::Instance()->isKeyDown(SDL_SCANCODE_SPACE)) {					// If spacebar is pressed - start playing game
			startGame();									// Advance to play game
			setButtonPressed();								// Disable ability to press button, and time before button can be pressed again
		}
		else if ((Input::Instance()->isKeyDown(SDL_SCANCODE_RETURN) ||				// If return key pressed
			Input::Instance()->getButtonState(0, 0))) {					// Or gamepad button A - select current highlighted button
			
			if (currentBtn == 1) startGame();						// 1. Start single player game
			else if (currentBtn == 2) connectToServer();					// 2. Connect to the server
			else if (currentBtn == 3) readyToStart();					// 3. Ready to start (all players connected)

			setButtonPressed();								// Disable ability to press button, and time before button can be pressed again
		}
	} 

	// Update buttons
	if (!m_gameObjects.empty()) {									// If the game object list isn't empty
		for (int i = 0; i < m_gameObjects.size(); i++) {
			if (m_gameObjects[i] != 0) {
				m_gameObjects[i]->update();						// Run update function for each object in m_gameObjects list
			}
		}
	}
//	else std::cout << "MainMenuState -> update() -> buttonPressed()" << std::endl;

	highlightCurrentButton(&m_gameObjects);								// Select the current button for keyboard / gamepad

	//if (Game::Instance()->getAssignedNetID() == 0)
	//	std::cout << "Waiting on netID" << std::endl;
}

void MainMenuState::render() {
	SDL_SetRenderDrawColor( Game::Instance()->getRenderer(), 0x00, 0x00, 0x00, 0xFF );		// Set clear colour
	SDL_RenderClear( Game::Instance()->getRenderer() );						// Clear screen

	if(m_loadingComplete && !m_gameObjects.empty()) {
		for(int i = 0; i < m_gameObjects.size(); i++) {
			m_gameObjects[i]->render();							// Call draw function for each object in m_gameObjects list
/*
			std::cout << "x: " << m_gameObjects[i]->getX();
			std::cout << " y: " << m_gameObjects[i]->getY();
			std::cout << " width: " << m_gameObjects[i]->getWidth();
			std::cout << " height: " << m_gameObjects[i]->getHeight();
			std::cout << " frame: " << m_gameObjects[i]->getCurrentFrame() << std::endl;
*/
		}
	}

	playerNetIDTexture.render((SCREEN_WIDTH - playerNetIDTexture.getWidth())/2,((SCREEN_HEIGHT-600-playerNetIDTexture.getHeight())/ 2)+600);
}

bool MainMenuState::onExit() {
	std::cout << "Exit Menu State" << std::endl;

	playerNetIDTexture.free();

	return true;
}

void MainMenuState::setCallbacks(const std::vector<Callback>& callbacks) {
    if(!m_gameObjects.empty()) {									// If its not empty
        for(int i = 0; i < m_gameObjects.size(); i++) {							// Go through the game objects list
            if(dynamic_cast<MenuButton*>(m_gameObjects[i])) {						// if they are of type MenuButton then assign a callback based on the id passed in from the file
                MenuButton* pButton = dynamic_cast<MenuButton*>(m_gameObjects[i]);
                pButton->setCallback(callbacks[pButton->getCallbackID()]);
		std::cout << "MainMenuState setCallBacks callback ID: " << pButton->getCallbackID() << std::endl;
            }
        }
    }
}

void MainMenuState::startGame() {
	// Assign netID to player as they connect to server

	Game::Instance()->getStateMachine()->changeState(new PlayState());				// Start the game
}

bool doOnce = true;

// Server assigns an integer identifier used for communicating
void MainMenuState::connectToServer() {
	while (doOnce) {
		std::cout << "MainMenuState ConnectToServer() button pressed" << std::endl;
		NetJOR::Instance()->sendString("0");							// Test new player connection on server

		//std::cout << "MainMenuState connectToServer() Test" << std::endl;

		char* input = NetJOR::Instance()->recvSrvMsg();

		// Parse NetID
		int playerID;
		char discard[100];	// don't need this bit

		sscanf(input, "%d %s", &playerID, &(*discard));	// parse

		char msg[100];
		//char* msg2 = "Server has designated you Player";
		snprintf(msg, 40, "Server has designated you as Player%d",playerID);


		SDL_Color textColor = { 255, 0, 0 };
		//if( !playerNetIDTexture.loadFromRenderedText( "Server has designated you Player", textColor )) {
		if( !playerNetIDTexture.loadFromRenderedText( msg, textColor )) {
			printf( "Failed to load player net id text texture!\n" );
		}

		Game::Instance()->setAssignedNetID(playerID);						// Store value to be accessed in play state

		//std::cout << "MainMenuState connectToServer()" << output << std::endl;
		//std::cout << "MainMenuState connectToServer() Test" << std::endl;

		// Set the button as grey
		
		doOnce = false;
	}
}

void MainMenuState::readyToStart() {
	std::cout << "MainMenuState readyToStart() button pressed" << std::endl;
}

