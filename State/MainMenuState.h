/*
	Joe O'Regan
	K00203642

	MainMenuState.h
*/
#ifndef MAIN_MENU_STATE_H
#define MAIN_MENU_STATE_H

#include <vector>
#include "MenuState.h"
#include "../GameObject.h"

class MainMenuState : public MenuState {
public:
    	virtual ~MainMenuState() {}

	virtual void handleInput();
	virtual void update();
	virtual void render();
		
	virtual bool onEnter();
	virtual bool onExit();

	virtual int getStateID() const { return MENU; };

private:    
	virtual void setCallbacks(const std::vector<Callback>& callbacks);

	std::vector<GameObject*> m_gameObjects;
//	std::vector<GameObject*> listOfMenuObjects;								// List of game objects
//	std::vector<MenuButton*> listOfMenuButtons;								// List of menu button objects

	static void connectToServer();
	static void readyToStart();
	static void startGame();
};

#endif
