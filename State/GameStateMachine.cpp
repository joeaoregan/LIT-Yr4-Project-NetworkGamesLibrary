#include "GameStateMachine.h"
#include <iostream>

void GameStateMachine::update() {
	if(!m_gameStates.empty()) {
		m_gameStates.back()->update();
	}
}

void GameStateMachine::render() {
	if(!m_gameStates.empty()) {
		m_gameStates.back()->render();
	}
}

/*
	Push pState into the states array, and call onEnter()
*/
 void GameStateMachine::pushState(GameState *pState) {
	m_gameStates.push_back(pState);
	m_gameStates.back()->onEnter();
}

/*
	If there is an available state, call current states onExit()
	Then remove the state from the states array
*/
void GameStateMachine::popState() {
	if(!m_gameStates.empty()) {					// If states array isn't empty
	 	if(m_gameStates.back()->onExit()) {			// and exiting the current state is completes successfully
			delete m_gamestates.back();			// remove the current state
			m_gameStates.pop_back();
		}
	}
	m_gameStates.back()->resume();
}

/*
	If there is a state in the array, and its ID isn't the same as the current state
	Remove the current state, add new state, and call onEnter()
*/
void GameStateMachine::changeState(GameState* pState) {
	if(!m_gameStates.empty()) { 						// If there is a state in the states array
		if(m_gameStates.back()->getStateID() == pState->getStateID())	// If the states IDs are the same
			return;							// no need to do anything
	

	 	if(m_gameStates.back()->onExit()) {
			delete m_gamestates.back();				// Remove current state
			m_gameStates.pop_back();
		}
	} 
	
	m_gameStates.push_back(pState);						// Push back new state

	m_gameStates.back()->OnEnter();						// Initialise 
}

void GameStateMachine::clean() {
    if(!m_gameStates.empty()) {
        m_gameStates.back()->onExit();

        delete m_gameStates.back();
        
        m_gameStates.clear();
    }
}


