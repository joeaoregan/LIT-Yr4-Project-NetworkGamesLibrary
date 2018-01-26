
#include "MainMenuState.h"
#include "../MenuButton.h"
#include "../Texture.h"
#include "../Background.h"
#include "PlayState.h"
#include "../MenuButton.h"
#include "../Input.h"

//Texture bg;
std::vector<GameObject*> listOfMenuObjects;								// List of game objects
std::vector<MenuButton*> listOfMenuButtons;								// List of game objects
GameObject* menuBackground;
//GameObject* menuButton1;
MenuButton* menuButton1;
MenuButton* menuButton2;
MenuButton* menuButton3;

bool MainMenuState::onEnter() {
	std::cout << "Loading Menu State" << std::endl;

	bool success = true;

	//numButtons = 3;
	currentBtn = 0;
	
	m_callbacks.push_back(0);
	m_callbacks.push_back(startGame);								// Add the game state to callbacks list

	setCallbacks(m_callbacks);									// set the callbacks for menu items

	success = Texture::Instance()->loadTextures(MENU_DATA);						// Load the menu textures
	if (success) std::cout << "Menu Textures Loaded" << std::endl;

	//menuBackground = new Background();
	//menuBackground->setTextureID("logoID");
	//listOfMenuObjects.push_back(menuBackground);

	menuButton1 = new MenuButton();
	menuButton2 = new MenuButton();
	menuButton3 = new MenuButton();

	menuButton2->setY(100);
	menuButton3->setY(200);
	menuButton1->setCallbackID(1);
	menuButton1->setCallbackID(2);
	menuButton1->setCallbackID(3);
	listOfMenuButtons.push_back(menuButton1);
	listOfMenuButtons.push_back(menuButton2);
	listOfMenuButtons.push_back(menuButton3);

	m_loadingComplete = true;

	return success;
}

SDL_Event e;

void MainMenuState::handleInput() {
	//menuButton1->handleEvents((Input::Instance()->getEvent()), 0);					// 0 = button 1 (put more buttons  in for loop

	for (int index = 0; index != listOfMenuButtons.size(); ++index) {	
		listOfMenuButtons[index]->handleEvents((Input::Instance()->getEvent()), index);		// Update the buttons
	}
}

void MainMenuState::update(){	
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
	
	if (!buttonPressed()) {
		MenuState::update();									// Up and down buttons

		if (Input::Instance()->isKeyDown(SDL_SCANCODE_SPACE)) {					// If spacebar is pressed - start playing game
			startGame();									// Advance to play game
			setButtonPressed();								// Disable ability to press button, and time before button can be pressed again
		}
		else if ((Input::Instance()->isKeyDown(SDL_SCANCODE_RETURN) ||				// If return key pressed
			Input::Instance()->getButtonState(0, 0))) {					// Or gamepad button A - select current highlighted button
			
			if (currentBtn == 1) startGame();
/*
			else if (currentBtn == 2) s_highScores();					// 2. High Scores
			else if (currentBtn == 3) s_settings();						// 3. Settings
			else if (currentBtn == 4) s_instructions();					// 4. Instructions
			else if (currentBtn == 5) s_exitFromMenu();					// 5. Exit Game
*/
			setButtonPressed();								// Disable ability to press button, and time before button can be pressed again
		}
	} 
}

void MainMenuState::render() {
	for (int index = 0; index != listOfMenuObjects.size(); ++index) {	
		listOfMenuObjects[index]->render();							// Render the game object
	}

	for (int index = 0; index != listOfMenuButtons.size(); ++index) {	
		listOfMenuButtons[index]->render();							// Render the game object
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
            }
        }
    }
}

void MainMenuState::startGame() {
	Game::Instance()->getStateMachine()->changeState(new PlayState());				// Start the game
}

