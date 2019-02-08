/*
	Joe O'Regan
	K00203642

	Input.h

	Cross-platform class for managing input from keyboard, mouse, and gamepad
*/

#ifndef __Input_Manager__
#define __Input_Manager__

// Unix (Ubuntu)
#if defined __linux__
#include <SDL2/SDL.h>						// SDL
// Windows
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#endif

#include <iostream>
#include <vector>

#include "Vector2D.h"

enum mouse_buttons { LEFT = 0, MIDDLE, RIGHT };

class Input {
public:    
	static Input* Instance() {
		if(s_pInstance == 0) {
			s_pInstance = new Input();
		}
        
		return s_pInstance;
	}
    
	// init joysticks
	void initialiseJoysticks();
	bool joysticksInitialised() const { return m_bJoysticksInitialised; }
    
	void reset();
    
	// update and clean the input handler
	void update();
	void clean();
    
	bool isKeyDown(SDL_Scancode key) const;				// keyboard events
    
	// joystick events
	int getAxisX(int joy, int stick) const;
	int getAxisY(int joy, int stick) const;
	bool getButtonState(int joy, int buttonNumber) const;

    
	// mouse events
	bool getMouseButtonState(int buttonNumber) const{ return m_mouseButtonStates[buttonNumber]; }
	Vector2D* getMousePosition() const { return m_mousePosition; }
	SDL_Event* getEvent(){ return &event; }

	SDL_Event event;

private:
    
	Input();
	~Input();
    
	Input(const Input&);
	Input& operator=(const Input&);
    
	// private functions to handle different event types
    
	// handle keyboard events
	void onKeyDown();
	void onKeyUp();
    
	// handle mouse events
	void onMouseMove(SDL_Event& event);
	void onMouseButtonDown(SDL_Event& event);
	void onMouseButtonUp(SDL_Event& event);
    
	// handle joysticks events
	void onJoystickAxisMove(SDL_Event& event);
	void onJoystickButtonDown(SDL_Event& event);
	void onJoystickButtonUp(SDL_Event& event);

	// member variables    
	
	const Uint8* m_keystates;					// keyboard specific
    
	// joystick specific
	std::vector< std::pair<Vector2D*, Vector2D*> > m_joystickValues;
	std::vector<SDL_Joystick*> m_joysticks;
	std::vector<std::vector<bool> > m_buttonStates;
	bool m_bJoysticksInitialised;
	static const int m_joystickDeadZone = 10000;
    
	// mouse specific
	std::vector<bool> m_mouseButtonStates;
	Vector2D* m_mousePosition;
	
	static Input* s_pInstance;					// singleton
};

typedef Input InputManager;

#endif //  __Input_Manager__
