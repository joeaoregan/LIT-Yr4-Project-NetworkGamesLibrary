/*
	Joe O'Regan
	K00203642

	MenuButton.cpp

	Cross-platfrom button class
*/

#include "Texture.h"
#include "MenuButton.h"
#include "Input.h"

MenuButton::MenuButton() : GameObject(), m_callback(0), m_bReleased(true) {
	SetParameters();
}

MenuButton::MenuButton(int callbackID) : GameObject(), m_callback(0), m_bReleased(true) {
	SetParameters();
	setCallbackID(callbackID);
}

MenuButton::MenuButton(int callbackID, int x, int y, std::string texture) : GameObject(), m_callback(0), m_bReleased(true) {
	SetParameters();
	setCallbackID(callbackID);
	setX(x);
	setY(y);
	setTextureID(texture);
}

void MenuButton::SetParameters() {
//	setX(0);
//	setY(0);
//	mPosition.x = 0;
//	mPosition.y = 0;
	setWidth(400);
	setHeight(100);
	setCurrentFrame(0);		// Sets int m_currentFrame. The current animation frame
//	setTextureID("playBtnID");
	setButtonSelected(false);	// Sets bool selected. The button has not been selected yet
}


/*
void MenuButton::load(std::unique_ptr<LoaderParams> const &pParams) {
	ShooterObject::load(std::move(pParams));
	m_callbackID = pParams->getCallbackID();
	m_currentFrame = MOUSE_OUT;
	selected = false;
}
*/
void MenuButton::render() {
	//std::cout << "MenuButton render()" << std::endl;
	GameObject::renderAnimation();
	
}
/*
void MenuButton::render(Texture &texture, SDL_Rect *currentClip) {
	//texture.render(mPosition.x, mPosition.y, currentClip);								// Show current button spriteCHANGED - RENDERER IS ADDED
	//texture.render(getX(), getY(), currentClip);								// Show current button spriteCHANGED - RENDERER IS ADDED
}
*/

void MenuButton::update() {

//	std::cout << "MenuButton update()" << std::endl;
/*
	Vector2D* pMousePos = Input::Instance()->getMousePosition();
    
	if(pMousePos->getX() < (m_position.getX() + m_width) && pMousePos->getX() > m_position.getX()			// If the cursor is outside the button
		&& pMousePos->getY() < (m_position.getY() + m_height) && pMousePos->getY() > m_position.getY()) {
		if(Input::Instance()->getMouseButtonState(LEFT) && m_bReleased) {					// mouse button pressed
			m_currentFrame = CLICKED;									// Set frame to clicked
            
			if(m_callback != 0)  {
			m_callback();
			}
            
			m_bReleased = false;
        	}
		//else if (!m_bReleased) m_currentFrame = MOUSE_OVER;
        	else if(!Input::Instance()->getMouseButtonState(LEFT))  {
			m_bReleased = true;
			m_currentFrame = MOUSE_OVER;
		}
	}
	else {
		m_currentFrame = MOUSE_OUT;
    	}
*/
	if (selected) m_currentFrame = MOUSE_OVER;

}

void MenuButton::clean() {
	 GameObject::clean();
}

//void MenuButton::testcase() {
//	m_currentFrame = MOUSE_OVER;
//}

void MenuButton::handleEvents(SDL_Event* e, int buttonSelected) {
	SDL_Color textColorOne = { 255, 255, 255 };	//If mouse event happened	

	//std::cout << "Texture ID: " << getTextureID() << std::endl;
	
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {
		//Get mouse position
		int x, y;											// Mouse coordinates
		SDL_GetMouseState(&x, &y);									// Get the mouse cursor position

		bool inside = true;										// Check if mouse is in button
/*
		if (x < mPosition.x) inside = false;								// Mouse is left of the button
		else if (x > mPosition.x + BUTTON_WIDTH) inside = false;					// Mouse is right of the button		
		else if (y < mPosition.y) inside = false;							// Mouse above the button		
		else if (y > mPosition.y + BUTTON_HEIGHT) inside = false;					// Mouse below the button
*/
		if (x < getX()) inside = false;									// Mouse is left of the button
		//else if (x > getX() + BUTTON_WIDTH) inside = false;						// Mouse is right of the button	
		else if (x > getX() + getWidth()) inside = false;						// Mouse is right of the button		
		else if (y < getY()) inside = false;								// Mouse above the button		
		//else if (y > getY() + BUTTON_HEIGHT) inside = false;						// Mouse below the button	
		else if (y > getY() + getHeight()) inside = false;						// Mouse below the button

		// Mouse is outside button
		if (!inside) {
			//setButtonSprite(BUTTON_SPRITE_MOUSE_OUT);						// Sprite 0
			setCurrentFrame(0);
		} else {
			switch (e->type) {
			case SDL_MOUSEMOTION:
				//std::cout << "case mouse motion" << std::endl;
				//setButtonSprite(BUTTON_SPRITE_MOUSE_OVER_MOTION);				// Set sprite mouse over 1 2017/03/24 Use setter method
				
				//if (getTextureID() != "logoID")
				//	setTextureID("logoID");
				
				setCurrentFrame(1);

				break;

			case SDL_MOUSEBUTTONDOWN:
				//setButtonSprite(BUTTON_SPRITE_MOUSE_DOWN);					// Set sprite mouse down 2 2017/03/24 Use setter method

				//if (getTextureID() != "playBtnID")
				//	setTextureID("playBtnID");

				if (!getButtonSelected()) {
					std::cout << "MenuButton.cpp handleEvents() Mouse Button Down" << std::endl;
					setCurrentFrame(2);
					setButtonSelected(true);							// The button has been selected
				}
				if(m_callback != 0)  {
					m_callback();
				}
 /*
				// Handle Main Menu Buttons
				if (buttonSelected == BUTTON1) {
					std::cout << "MenuButton.cpp handleEvents() Selected: Start Game!" << std::endl;
				}
				else if (buttonSelected == BUTTON2) {
					std::cout << "MenuButton.cpp handleEvents() Selected: Connect To Server!" << std::endl;
				}
				else if (buttonSelected == BUTTON3) {
					std::cout << "MenuButton.cpp handleEvents() Selected: Ready To Start!" << std::endl;
				}
*/
				break;

			case SDL_MOUSEBUTTONUP:
				//setButtonSprite(BUTTON_SPRITE_MOUSE_UP);					 // Set sprite mouse up 3 2017/03/24 Use setter method
				//std::cout << "Mouse Button Up" << std::endl;

				//std::cout << "Option selected" << std::endl;
				
				// GO TO MENU OPTION
				break;
			}
		}
	}
}

