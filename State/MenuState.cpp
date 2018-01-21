#include "MenuState.h"

void MenuState::update(){
	
}

void MenuState::render() {

}

bool MenuState::onEnter() {
	std::cout << "Loading Menu State" << std::endl;
	return true;
}

bool MenuState::onExit() {
	std::cout << "Exit Menu State" << std::endl;
	return true;
}
