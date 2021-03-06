/*
	Joe O'Regan
	K00203642

	MenuButton.h

	Cross-platfrom button class
*/

#ifndef MENU_BUTTON_H
#define MENU_BUTTON_H

// Unix (Ubuntu)
#if defined __linux__
#include <SDL2/SDL.h>						// SDL
// Windows
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#endif

//#include "ShooterObject.h"
#include "GameObject.h"
//#include "GameObjectFactory.h"

//const int BUTTON_WIDTH = 400;
//const int BUTTON_HEIGHT = 100;

enum ButtonSprite {
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
};

//enum mainMenuButtons { STORY, GAME_1PLAYER, GAME_2PLAYER, GO_TO_SETTINGS, MENU_TO_HIGH_SCORES, QUIT };
enum mainMenuButtons { BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6 };

class MenuButton : public GameObject {
public:    
	MenuButton();
	MenuButton(int callback);
	MenuButton(int callback, int x, int y, std::string texture);

	void SetParameters();

	virtual ~MenuButton() {}

	//virtual void load(std::unique_ptr<LoaderParams> const &pParams);

	SDL_Point mPosition;

	virtual void render();
	//void render(Texture &texture, SDL_Rect *currentClip);		// Rendering function
	virtual void update();
	virtual void clean();
	
	void handleEvents(SDL_Event* e, int buttonSelected);		// Handles button events

	void setCallback(void(*callback)()) { m_callback = callback;}

	int getCallbackID() { return m_callbackID; }
	void setCallbackID(int set) { m_callbackID = set;}

	//void testcase();

	bool selected;							// The button has been selected by keyboard / gamepad
	bool m_bReleased;						// Mouse over selected or not

	ButtonSprite getButtonSprite() { return mCurrentSprite; }	// Get the button sprite
	void setButtonSprite(ButtonSprite bs) { mCurrentSprite = bs; }	// Set the button sprite

	bool getButtonSelected() { return buttonSelected; }
	void setButtonSelected(bool set) { buttonSelected = set; }

private:    
	ButtonSprite mCurrentSprite;					// Currently used global sprite - Sprite enumeration

	enum button_state { MOUSE_OUT, MOUSE_OVER, CLICKED };

	//bool m_bReleased;

	int m_callbackID;

	void (*m_callback)();

	bool buttonSelected;
};
/*
class MenuButtonCreator : public BaseCreator {
    GameObject* createGameObject() const {
        return new MenuButton();
    }
};
*/


#endif /* defined(__SDL_Game_Programming_Book__MenuObject__) */

