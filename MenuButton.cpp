/*
	MenuButton.cpp
*/

#include "Texture.h"
#include "MenuButton.h"
#include "Input.h"

MenuButton::MenuButton() : GameObject(), m_callback(0), m_bReleased(true) {
	setX(0);
	setY(0);
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
	GameObject::renderAnimation();
//	SDL_Rect renderQuad = { getX(), getY(), getWidth(), getHeight() };								// Set rendering space and render to screen
//	SDL_RenderCopyEx(Game::Instance()->getRenderer(), Texture::Instance()->getTexture(getTextureID()), NULL, &renderQuad, 0, NULL, SDL_FLIP_NONE);	// Render to screen
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

