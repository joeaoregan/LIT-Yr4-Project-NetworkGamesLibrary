/*
	Joe O'Regan
	K00203642

	GameState.h

	Abstract state base class
*/

#include <string>
#include <vector>

enum StateIDs{
	MENU = 0,
	PLAY,
	PAUSE,
	GAMEOVER
};

class GameState {
public:
	virtual void update() = 0;		// = 0 -> has to be implemented in derived class
	virtual void render() = 0;
		
	virtual bool onEnter() = 0;
	virtual bool onExit() = 0;

	virtual void resume() {}

	virtual int getStateID() const = 0;
};
