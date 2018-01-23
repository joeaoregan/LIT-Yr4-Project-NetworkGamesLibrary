
#include "MainMenuState.h"
#include "../MenuButton.h"
#include "../Texture.h"
#include "../Background.h"

//Texture bg;
std::vector<GameObject*> listOfMenuObjects;								// List of game objects
GameObject* menuBackground;

bool MainMenuState::onEnter() {
	std::cout << "Loading Menu State" << std::endl;
	bool success = true;

	success = Texture::Instance()->loadTextures(MENU_DATA);						// Load the menu textures
	if (success) std::cout << "Menu Textures Loaded" << std::endl;

	menuBackground = new Background();
	menuBackground->setTextureID("logoID");
	listOfMenuObjects.push_back(menuBackground);

	return success;
}

void MainMenuState::update(){	
	for (int index = 0; index != listOfMenuObjects.size(); ++index) {	
		listOfMenuObjects[index]->update();							// Update the game objects
	}
	
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
