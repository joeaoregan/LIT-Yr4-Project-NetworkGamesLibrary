
#include "MainMenuState.h"
#include "../MenuButton.h"
#include "../Texture.h"
#include "../Background.h"
#include "PlayState.h"
#include "../MenuButton.h"
#include "../Input.h"

#include "../Networking/ServerUDP/Text.h"

//Texture bg;
GameObject* menuBackground;
//GameObject* menuButton1;
GameObject* menuButton1;
GameObject* menuButton2;
GameObject* menuButton3;

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
	menuButton2->setTextureID("connectBtnID");
	menuButton3->setTextureID("readyBtnID");

	menuButton2->setY(100);
	menuButton3->setY(200);
/*
	listOfMenuButtons.push_back(menuButton1);
	listOfMenuButtons.push_back(menuButton2);
	listOfMenuButtons.push_back(menuButton3);
*/
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

	return success;
}

SDL_Event e;

void MainMenuState::handleInput() {
/*
	for (int index = 0; index != listOfMenuButtons.size(); ++index) {	
		listOfMenuButtons[index]->handleEvents((Input::Instance()->getEvent()), index);		// Update the buttons
	}
*/
	for (int index = 0; index != m_gameObjects.size(); ++index) {	
		dynamic_cast<MenuButton*>(m_gameObjects[index])->handleEvents((Input::Instance()->getEvent()), index);		// Update the input
	}

	// GameObject has no handleEvents() function

}

void MainMenuState::update(){	
/*
	// Update objects
	for (int index = 0; index != listOfMenuObjects.size(); ++index) {	
		listOfMenuObjects[index]->update();	
	}

	// Update buttons
	for (int index = 0; index != listOfMenuButtons.size(); ++index) {	
		listOfMenuButtons[index]->update();							// Update the game objects

		if (listOfMenuButtons[index]->getButtonSelected() && currentBtn == 0)  {
			listOfMenuButtons[index]->setButtonSelected(false);				// Reset the button for when the menu is entered again
			startGame();
		}
	}
*/
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
/*
			if (dynamic_cast<MenuButton*>(m_gameObjects[i])->getButtonSelected() && currentBtn == 1)  {
				dynamic_cast<MenuButton*>(m_gameObjects[i])->setButtonSelected(false);				// Reset the button for when the menu is entered again
				startGame();
			}
			if (dynamic_cast<MenuButton*>(m_gameObjects[i])->getButtonSelected() && currentBtn == 2)  {
				dynamic_cast<MenuButton*>(m_gameObjects[i])->setButtonSelected(false);				// Reset the button for when the menu is entered again
				connectToServer();
			}
			if (dynamic_cast<MenuButton*>(m_gameObjects[i])->getButtonSelected() && currentBtn == 3)  {
				dynamic_cast<MenuButton*>(m_gameObjects[i])->setButtonSelected(false);				// Reset the button for when the menu is entered again
				readyToStart();
			}
*/
		}
	}

//	else std::cout << "MainMenuState -> update() -> buttonPressed()" << std::endl;

	highlightCurrentButton(&m_gameObjects);								// Select the current button for keyboard / gamepad
}

void MainMenuState::render() {
/*
	for (int index = 0; index != listOfMenuObjects.size(); ++index) {	
		listOfMenuObjects[index]->render();							// Render the game object
	}

	for (int index = 0; index != listOfMenuButtons.size(); ++index) {	
		listOfMenuButtons[index]->render();							// Render the game object
	}
*/
									
    if(m_loadingComplete && !m_gameObjects.empty()) {
        for(int i = 0; i < m_gameObjects.size(); i++) {
            m_gameObjects[i]->render();									// Call draw function for each object in m_gameObjects list
        }
    }
}

bool MainMenuState::onExit() {
	std::cout << "Exit Menu State" << std::endl;

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
	Game::Instance()->getStateMachine()->changeState(new PlayState());				// Start the game
}

void MainMenuState::connectToServer() {
	std::cout << "MainMenuState ConnectToServer() button pressed" << std::endl;
}

void MainMenuState::readyToStart() {
	std::cout << "MainMenuState readyToStart() button pressed" << std::endl;
}

