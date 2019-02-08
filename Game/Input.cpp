/*
	Joe O'Regan
	K00203642

	Input.cpp

	Cross-platform class for managing input from keyboard, mouse, and gamepad
*/

#include "Input.h"
#include "Game.h"
#include <iostream>

Input* Input::s_pInstance = 0;

Input::Input() :  m_keystates(0),
m_bJoysticksInitialised(false), 
m_mousePosition(new Vector2D(0,0)) {
	// create button states for the mouse
	for(int i = 0; i < 3; i++) {
		m_mouseButtonStates.push_back(false);
	}
}

Input::~Input() {
	// delete anything we created dynamically
	delete m_keystates;
	delete m_mousePosition;

	// clear our arrays
	m_joystickValues.clear();
	m_joysticks.clear();
	m_buttonStates.clear();
	m_mouseButtonStates.clear();
}

void Input::clean() {
	// we need to clean up after ourselves and close the joysticks we opened
	if(m_bJoysticksInitialised) {
		for(int i = 0; i < SDL_NumJoysticks(); i++) {
			SDL_JoystickClose(m_joysticks[i]);
		}
	}
}

void Input::initialiseJoysticks() {
	// if we haven't already initialised the joystick subystem, we will do it here
	if(SDL_WasInit(SDL_INIT_JOYSTICK) == 0) {
		SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	}
    
	// get the number of joysticks, skip init if there aren't any
	if(SDL_NumJoysticks() > 0) {
		for(int i = 0; i < SDL_NumJoysticks(); i++) {

			SDL_Joystick* joy = SDL_JoystickOpen(i);						// create a new joystick

			// if the joystick opened correctly we need to populate our arrays
			if(SDL_JoystickOpen(i)) {

				m_joysticks.push_back(joy);							// push back into the array to be closed later  

				m_joystickValues.push_back(std::make_pair(new Vector2D(0,0),new Vector2D(0,0)));// create a pair of values for the axes, a vector for each stick
					       
				std::vector<bool> tempButtons;							// create an array to hold the button values

				// fill the array with a false value for each button
				for(int j = 0; j <  SDL_JoystickNumButtons(joy); j++) {
					tempButtons.push_back(false);
				}

				m_buttonStates.push_back(tempButtons);						// push the button array into the button state array
			} else {                
				std::cout << SDL_GetError();							// if there was an error initialising a joystick we want to know about it
			}
		}

		// enable joystick events
		SDL_JoystickEventState(SDL_ENABLE);
		m_bJoysticksInitialised = true;

		std::cout << "Initialised " << m_joysticks.size() << " joystick(s)";
	} else {
		m_bJoysticksInitialised = false;
	}
}

void Input::reset() {
	m_mouseButtonStates[LEFT] = false;
	m_mouseButtonStates[RIGHT] = false;
	m_mouseButtonStates[MIDDLE] = false;
}

bool Input::isKeyDown(SDL_Scancode key) const {
	//std::cout << "Input iskeyDown()" << std::endl;
	if(m_keystates != 0) {
		if(m_keystates[key] == 1) {
			//std::cout << "key down" << std::endl;
			return true;
		} else {
			return false;
		}
	}
    
	//std::cout << "Input iskeyDown() end " << std::endl;
	return false;
}

int Input::getAxisX(int joy, int stick) const {
	if(m_joystickValues.size() > 0) {
		if(stick == 1) {
			return (int) m_joystickValues[joy].first->getX();
		} else if(stick == 2) {
			return (int) m_joystickValues[joy].second->getX();
		}
	}

	return 0;
}

int Input::getAxisY(int joy, int stick) const {
	if(m_joystickValues.size() > 0) {
		if(stick == 1) {
			return (int) m_joystickValues[joy].first->getY();
		} else if(stick == 2) {
			return (int) m_joystickValues[joy].second->getY();
		}
	}

	return 0;
}

bool Input::getButtonState(int joy, int buttonNumber) const {
	//std::cout << "Input getButtonState()" << std::endl;
	if (SDL_NumJoysticks() > 0)								// If there is a joystick plugged in
		return m_buttonStates[joy][buttonNumber];					// Return it's state
		
	return false;										// Otherwise return false
}

void Input::update() {
	//SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch (event.type) {
			//case SDL_QUIT: Game::Instance()->quit(); break;   
			//case SDL_QUIT: Game::Instance()->close(); break;    
			case SDL_QUIT: Game::Instance()->setGameFinished(); std::cout << "Input quit" << std::endl; break;                
			case SDL_JOYAXISMOTION: onJoystickAxisMove(event); break;                
			case SDL_JOYBUTTONDOWN: onJoystickButtonDown(event); 
			case SDL_JOYBUTTONUP: onJoystickButtonUp(event); break;                
			case SDL_MOUSEMOTION: onMouseMove(event); break;                
			case SDL_MOUSEBUTTONDOWN: onMouseButtonDown(event); break;                
			case SDL_MOUSEBUTTONUP: onMouseButtonUp(event); break;                
			case SDL_KEYDOWN: onKeyDown(); break;                
			case SDL_KEYUP: onKeyUp(); break;                
			default: break;
		}
	}
}

void Input::onKeyDown() {
	m_keystates = SDL_GetKeyboardState(0);
}

void Input::onKeyUp() {
	m_keystates = SDL_GetKeyboardState(0);
}

void Input::onMouseMove(SDL_Event &event) {
	m_mousePosition->setX((float) event.motion.x);
	m_mousePosition->setY((float) event.motion.y);
}

void Input::onMouseButtonDown(SDL_Event &event) {
	if(event.button.button == SDL_BUTTON_LEFT) {
		m_mouseButtonStates[LEFT] = true;
	}

	if(event.button.button == SDL_BUTTON_MIDDLE) {
		m_mouseButtonStates[MIDDLE] = true;
	}

	if(event.button.button == SDL_BUTTON_RIGHT) {
		m_mouseButtonStates[RIGHT] = true;
	}
}

void Input::onMouseButtonUp(SDL_Event &event) {
	if(event.button.button == SDL_BUTTON_LEFT) {
		m_mouseButtonStates[LEFT] = false;
	}

	if(event.button.button == SDL_BUTTON_MIDDLE) {
		m_mouseButtonStates[MIDDLE] = false;
	}

	if(event.button.button == SDL_BUTTON_RIGHT) {
		m_mouseButtonStates[RIGHT] = false;
	}
}

void Input::onJoystickAxisMove(SDL_Event &event) {
	int whichOne = event.jaxis.which;
	// left stick move left or right
	if(event.jaxis.axis == 0) {
		if (event.jaxis.value > m_joystickDeadZone) {
			m_joystickValues[whichOne].first->setX(1);
		} else if(event.jaxis.value < -m_joystickDeadZone) {
			m_joystickValues[whichOne].first->setX(-1);
		} else {
			m_joystickValues[whichOne].first->setX(0);
		}
	}

	// left stick move up or down
	if(event.jaxis.axis == 1) {
		if (event.jaxis.value > m_joystickDeadZone) {
			m_joystickValues[whichOne].first->setY(1);
		} else if(event.jaxis.value < -m_joystickDeadZone) {
		 	m_joystickValues[whichOne].first->setY(-1);
		} else {
			m_joystickValues[whichOne].first->setY(0);
		}
	}

	// right stick move left or right
	if(event.jaxis.axis == 3) {
		if (event.jaxis.value > m_joystickDeadZone) {
			m_joystickValues[whichOne].second->setX(1);
		} else if(event.jaxis.value < -m_joystickDeadZone) {
			m_joystickValues[whichOne].second->setX(-1);
		} else {
			m_joystickValues[whichOne].second->setX(0);
		}
	}

	// right stick move up or down
	if(event.jaxis. axis == 4) {
		if (event.jaxis.value > m_joystickDeadZone) {
			m_joystickValues[whichOne].second->setY(1);
		} else if(event.jaxis.value < -m_joystickDeadZone) {
			m_joystickValues[whichOne].second->setY(-1);
		} else {
			m_joystickValues[whichOne].second->setY(0);
		}
	}
}

void Input::onJoystickButtonDown(SDL_Event &event) {
	int whichOne = event.jaxis.which;

	m_buttonStates[whichOne][event.jbutton.button] = true;
}

void Input::onJoystickButtonUp(SDL_Event &event) {
	int whichOne = event.jaxis.which;

	m_buttonStates[whichOne][event.jbutton.button] = false;
}
