/*
	Joe O'Regan
	K00203642

	GameState.h

	Abstract state base class
*/

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <string>
#include <vector>
#include <iostream>

enum StateIDs{
	MENU = 0,
	PLAY,
	PAUSE,
	GAMEOVER
};

class GameState {
public:
	virtual void handleInput() = 0;
	virtual void update() = 0;					// = 0 -> has to be implemented in derived class
	virtual void render() = 0;
		
	virtual bool onEnter() = 0;
	virtual bool onExit() = 0;

	virtual void resume() {}

	virtual int getStateID() const = 0;

protected:
	GameState() : m_loadingComplete(false), m_exiting(false) { }	// Constructor

	bool m_loadingComplete;						// Finished loading textures etc
    	bool m_exiting;	
};

#endif
