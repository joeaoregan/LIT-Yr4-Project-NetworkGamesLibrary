#include "PlayState.h"

void PlayState::update(){
	
}

void PlayState::render() {

}

bool PlayState::onEnter() {
	std::cout << "Loading Play State" << std::endl;
	return true;
}

bool PlayState::onExit() {
	std::cout << "Exit Play State" << std::endl;
	return true;
}
