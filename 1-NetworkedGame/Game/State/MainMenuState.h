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
		
	virtual bool onEnter();							// Initialise the scene
	virtual bool onExit();							// Leave the scene

	virtual int getStateID() const { return MENU; };			// State identifier

private:    
	virtual void setCallbacks(const std::vector<Callback>& callbacks);

	std::vector<GameObject*> m_gameObjects;					// List of game objects

	static void connectToServer();
	static void readyToStart();
	static void startGame();
};

#endif
