
#include "MainMenuState.h"
#include "../MenuButton.h"

void MainMenuState::update(){
	
}

void MainMenuState::render() {

}

bool MainMenuState::onEnter() {
	std::cout << "Loading Menu State" << std::endl;
	return true;
}

bool MainMenuState::onExit() {
	std::cout << "Exit Menu State" << std::endl;
	return true;
}

void MainMenuState::setCallbacks(const std::vector<Callback>& callbacks) {
    if(!m_gameObjects.empty()) {													// If its not empty
        for(int i = 0; i < m_gameObjects.size(); i++) {								// Go through the game objects list
            if(dynamic_cast<MenuButton*>(m_gameObjects[i])) {						// if they are of type MenuButton then assign a callback based on the id passed in from the file
                MenuButton* pButton = dynamic_cast<MenuButton*>(m_gameObjects[i]);
                pButton->setCallback(callbacks[pButton->getCallbackID()]);
            }
        }
    }
}
