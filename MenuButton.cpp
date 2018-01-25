/*
	MenuButton.cpp
*/

#include "Texture.h"
#include "MenuButton.h"
#include "Input.h"


MenuButton::MenuButton() : GameObject(), m_callback(0), m_bReleased(true) {
	setX(0);
	setY(0);
//	mPosition.x = 0;
//	mPosition.y = 0;
	setWidth(400);
	setHeight(100);
	setTextureID("playBtnID");
	setCurrentFrame(0);
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
	//GameObject::render();	
	setTextureID("playBtnID");
	GameObject::renderAnimation();
	//SDL_Rect renderQuad = { getX(), getY(), getWidth(), getHeight() };								// Set rendering space and render to screen
	//SDL_RenderCopyEx(Game::Instance()->getRenderer(), Texture::Instance()->getTexture(getTextureID()), NULL, &renderQuad, 0, NULL, SDL_FLIP_NONE);	// Render to screen
	
}

void MenuButton::render(Texture &texture, SDL_Rect *currentClip) {
	texture.render(mPosition.x, mPosition.y, currentClip);	// Show current button spriteCHANGED - RENDERER IS ADDED
}


void MenuButton::update() {
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

	if (selected) m_currentFrame = CLICKED;
*/
}

void MenuButton::clean() {
	 GameObject::clean();
}

void MenuButton::testcase() {
	m_currentFrame = MOUSE_OVER;
}

void MenuButton::handleEvents(SDL_Event* e, int buttonSelected) {
	SDL_Color textColorOne = { 255, 255, 255 };	//If mouse event happened	

	
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {
	//std::cout << "test1" << std::endl;
		//Get mouse position
		//MainMenu menu;
		int x, y;
		SDL_GetMouseState(&x, &y);

		bool inside = true;										// Check if mouse is in button
/*
		if (x < mPosition.x) inside = false;								// Mouse is left of the button
		else if (x > mPosition.x + BUTTON_WIDTH) inside = false;					// Mouse is right of the button		
		else if (y < mPosition.y) inside = false;							// Mouse above the button		
		else if (y > mPosition.y + BUTTON_HEIGHT) inside = false;					// Mouse below the button
*/
		if (x < getX()) inside = false;									// Mouse is left of the button
		else if (x > getX() + BUTTON_WIDTH) inside = false;						// Mouse is right of the button		
		else if (y < getY()) inside = false;								// Mouse above the button		
		else if (y > getY() + BUTTON_HEIGHT) inside = false;						// Mouse below the button

		//Mouse is outside button
		if (!inside) {
			setButtonSprite(BUTTON_SPRITE_MOUSE_OUT);						// Sprite 0
		} else {
			switch (e->type) {
			case SDL_MOUSEMOTION:
				std::cout << "case mouse motion" << std::endl;
				setButtonSprite(BUTTON_SPRITE_MOUSE_OVER_MOTION);				// Set sprite mouse over 1 2017/03/24 Use setter method
				
				setTextureID("player1ID");
				break;

			case SDL_MOUSEBUTTONDOWN:
				setButtonSprite(BUTTON_SPRITE_MOUSE_DOWN);					// Set sprite mouse down 2 2017/03/24 Use setter method
				//std::cout << "Mouse Button Down" << std::endl;

				setTextureID("player1ID");
				// Handle Main Menu Buttons
				if (buttonSelected == STORY) {
					std::cout << "Selected: Start Game!" << std::endl;
				}

			case SDL_MOUSEBUTTONUP:
				setButtonSprite(BUTTON_SPRITE_MOUSE_UP);					 // Set sprite mouse up 3 2017/03/24 Use setter method
				std::cout << "Mouse Button Up" << std::endl;

				// GO TO MENU OPTION
				break;
			}
		}
	}
}

