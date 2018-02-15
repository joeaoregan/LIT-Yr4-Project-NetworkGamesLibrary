
#ifndef GAME_STATE_MACHINE_H
#define GAME_STATE_MACHINE_H

#include "GameState.h"

class GameStateMachine {
public:    
	GameStateMachine() {}
	~GameStateMachine() {}
    
	void handleInput();
	void update();
	void render();

	void pushState(GameState* pState);	// Add state without removing previous state
	void changeState(GameState* pState);	// Remove previous state first before adding
	void popState();			// Remove the state without adding another state
    
	void clean();

	std::vector<GameState*>& getGameStates() { return m_gameStates; }

private:
	std::vector<GameState*> m_gameStates;
};

#endif
