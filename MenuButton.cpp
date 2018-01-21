/*
	MenuButton.cpp
*/

#include "MenuButton.h"
#include "Input.h"

MenuButton::MenuButton() : GameObject(), m_callback(0), m_bReleased(true) {
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
    GameObject::render();
}

void MenuButton::update() {
/*
    Vector2D* pMousePos = Input::Instance()->getMousePosition();
    
    if(pMousePos->getX() < (m_position.getX() + m_width) && pMousePos->getX() > m_position.getX()	// If the cursor is outside the button
       && pMousePos->getY() < (m_position.getY() + m_height) && pMousePos->getY() > m_position.getY()) {
        if(Input::Instance()->getMouseButtonState(LEFT) && m_bReleased) {				// mouse button pressed
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

