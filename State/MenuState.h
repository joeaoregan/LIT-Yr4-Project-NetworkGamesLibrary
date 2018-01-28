/*
	Joe O'Regan
	K00203642

	MenuState.h

	Cross-platform menu state
*/

#ifndef MENU_STATE_H
#define MENU_STATE_H

// Unix (Ubuntu)
#if defined __linux__
#include <SDL2/SDL.h>						// SDL
// Windows
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#endif

#include "GameState.h"
#include "../Input.h"
#include "../GameObject.h"
#include "../MenuButton.h"
#include "../Audio.h"

static const int TIME_BETWEEN_BUTTON_PRESSES = 200;

enum buttonMove {BUTTON_UP, BUTTON_DOWN};								// Enum to make gamepad / keyboard button selection more readable

class MenuState : public GameState {
public:    
	virtual ~MenuState() {
		//btnTimer = 0;
	}
	
	void setCurrentBtn(int a) {
		if (a == BUTTON_UP) currentBtn--;							// Highligh button higher up
		else if (a == BUTTON_DOWN) currentBtn++;						// Highlight button lower

		if (currentBtn > numButtons) currentBtn = 1;
		if (currentBtn < 1) currentBtn = numButtons;
	}

	virtual void handleInput() {}

	virtual void update() {
		//if (!buttonPressed()) {		
			if (Input::Instance()->isKeyDown(SDL_SCANCODE_UP) ||				// If up key, 
				Input::Instance()->getAxisY(0, 1) < 0) {				// Or gamepad up pressed
				setCurrentBtn(BUTTON_UP);						// Set the current button as up
				Audio::Instance()->playFX("buttonFX");					// Play button effect, don't loop
				setButtonPressed();							// Disable ability to press button, and time before button can be pressed again
			}
			else if (Input::Instance()->isKeyDown(SDL_SCANCODE_DOWN) ||			// If down key
				Input::Instance()->getAxisY(0, 1) > 0) {				// Or gamepad down pressed
				setCurrentBtn(BUTTON_DOWN);						// Set the current button as down
				Audio::Instance()->playFX("buttonFX");					// Play button effect, don't loop
				setButtonPressed();							// Disable ability to press button, and time before button can be pressed again
			}
		//}
	}
    	
	// Mark button has been pressed
	void setButtonPressed() {
		btnTimer = SDL_GetTicks();								// Set The timer to the current game time
		//pressed = true;									// Set the button pressed to true

		std::cout << "currentButton " << currentBtn << std::endl;
	}

	bool buttonPressed() {
		//std::cout << "button pressed 1" << std::endl;
		
		if (SDL_GetTicks() > btnTimer + TIME_BETWEEN_BUTTON_PRESSES) {				// If time since last button pressed is 1/4 of a second
			btnTimer = SDL_GetTicks();							// Reset time between button presses

			return false;									// OK to accept button press
		}

		//std::cout << "button pressed 2" << std::endl;

		return true;										// Not OK to accept button press
	}

protected:    
    typedef void(*Callback)();
    virtual void setCallbacks(const std::vector<Callback>& callbacks) = 0;

	virtual void highlightCurrentButton(const std::vector<GameObject *> *pObjects) {
		if (!pObjects->empty()) {								// If list of game objects is not empty
//		std::cout << "highlightCurrentButton()" << std::endl;
			for (int i = 0; i < pObjects->size(); i++) {					// Go through the game objects list
				if (dynamic_cast<MenuButton*>((*pObjects)[i])) {			// if they are of type MenuButton then assign a callback based on the id passed in from the file
					MenuButton* pButton = dynamic_cast<MenuButton*>((*pObjects)[i]);

					if (pButton->getCallbackID() == currentBtn) {
						pButton->selected = true;
					}
					else {
						pButton->selected = false;
					}
				}
			}
		}
//else std::cout << "highlightCurrentButton() pObjects empty" << std::endl;
	}

	std::vector<Callback> m_callbacks;

	unsigned int btnTimer;										// 2017/04/24 Delay between buttons. Moved from MainMenuState to be used by all menus
	bool pressed;

	int currentBtn;											// Current selected menu button for keyboard / gamepad // 2017/04/24 Moved from Main Menu State
	int numButtons;											// The number of buttons in the menu/state
};

#endif

