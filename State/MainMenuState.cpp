
#include "MainMenuState.h"
#include "../MenuButton.h"
#include "../Texture.h"
#include "../Background.h"
#include "PlayState.h"
#include "../MenuButton.h"

//Texture bg;
std::vector<GameObject*> listOfMenuObjects;								// List of game objects
GameObject* menuBackground;
GameObject* menuButton1;

bool MainMenuState::onEnter() {
	std::cout << "Loading Menu State" << std::endl;
	bool success = true;

	numButtons = 1;
	currentBtn = 1;
	
	m_callbacks.push_back(0);
	m_callbacks.push_back(startGame);								// Add the game state to callbacks list

	setCallbacks(m_callbacks);									// set the callbacks for menu items

	success = Texture::Instance()->loadTextures(MENU_DATA);						// Load the menu textures
	if (success) std::cout << "Menu Textures Loaded" << std::endl;

	menuBackground = new Background();
	menuBackground->setTextureID("logoID");
	listOfMenuObjects.push_back(menuBackground);

	menuButton1 = new MenuButton();
	//menuButton1->setX(0);
	//menuButton1->setY(0);
	//menuButton1->setWidth(1200);
	//menuButton1->setHeight(100);
	listOfMenuObjects.push_back(menuButton1);

	//std::cout << "test1" << std::endl;
	m_loadingComplete = true;

	return success;
}

void MainMenuState::update(){	
	//std::cout << "test2" << std::endl;
	for (int index = 0; index != listOfMenuObjects.size(); ++index) {	
		listOfMenuObjects[index]->update();							// Update the game objects
	}
	
	//std::cout << "test3" << std::endl;

	if (!buttonPressed()) {
	//std::cout << "test before menustate update" << std::endl;
		MenuState::update();									// Up and down buttons

	//std::cout << "test after menustate update" << std::endl;

		if (Input::Instance()->isKeyDown(SDL_SCANCODE_SPACE)) {					// If spacebar is pressed - start playing game
	//std::cout << "test before startGame(0" << std::endl;
			startGame();									// Advance to play game
			setButtonPressed();								// Disable ability to press button, and time before button can be pressed again
	//std::cout << "test4" << std::endl;
		}
		else if ((Input::Instance()->isKeyDown(SDL_SCANCODE_RETURN) ||				// If return key pressed
			Input::Instance()->getButtonState(0, 0))) {					// Or gamepad button A - select current highlighted button
			
	//std::cout << "test5" << std::endl;
			if (currentBtn == 1) startGame();
	//std::cout << "test6" << std::endl;
/*
			if (currentBtn == 1) s_menuToPlay();						// 1. Play Game
			else if (currentBtn == 2) s_highScores();					// 2. High Scores
			else if (currentBtn == 3) s_settings();						// 3. Settings
			else if (currentBtn == 4) s_instructions();					// 4. Instructions
			else if (currentBtn == 5) s_exitFromMenu();					// 5. Exit Game
*/
			setButtonPressed();								// Disable ability to press button, and time before button can be pressed again
		}
	} 

	//std::cout << "test end update" << std::endl;
}

void MainMenuState::render() {
	for (int index = 0; index != listOfMenuObjects.size(); ++index) {	
		listOfMenuObjects[index]->render();							// Render the game object
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
