/*
	Joe O'Regan
	K00203642

	PlayState.h
*/

#include "GameState.h"

class PlayState : public GameState {
	public:
		virtual void update();
		virtual void render();
		
		virtual bool onEnter();
		virtual bool onExit();

		virtual int getStateID() const { return PLAY; };
}
