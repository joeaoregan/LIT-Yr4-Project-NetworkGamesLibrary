/*
	Joe O'Regan
	K00203642

	PlayState.h
*/

#ifndef	PLAY_STATE_H
#define	PLAY_STATE_H

#include "GameState.h"
#include "../Game.h"

class PlayState : public GameState {
public:
	virtual void update();
	virtual void render();
		
	virtual bool onEnter();
	virtual bool onExit();

	virtual void resume() {};

	virtual int getStateID() const { return PLAY; };

private:
	int prevX;					// Players previous position
	int prevY;

	SDL_Event e;					// Event handler
};

#endif
