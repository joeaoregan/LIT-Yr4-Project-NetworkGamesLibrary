/*
	Joe O'Regan
	K00203642

	MenuState.h
*/

#include "GameState.h"

class MenuState : public GameState {
	public:
		virtual void update();
		virtual void render();
		
		virtual bool onEnter();
		virtual bool onExit();

		virtual int getStateID() const { return MENU; };
}
