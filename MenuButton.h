/*
	MenuButton.h
*/

#ifndef MENU_BUTTON_H
#define MENU_BUTTON_H

//#include "ShooterObject.h"
#include "GameObject.h"
//#include "GameObjectFactory.h"

//class MenuButton : public ShooterObject {
class MenuButton : public GameObject {
public:    
    MenuButton();
    
    virtual ~MenuButton() {}
    
//    virtual void load(std::unique_ptr<LoaderParams> const &pParams);
    
    virtual void render();
    virtual void update();
    virtual void clean();
    
    void setCallback(void(*callback)()) { m_callback = callback;}
    int getCallbackID() { return m_callbackID; }

	void testcase();

	bool selected;		// The button has been selected by keyboard / gamepad
	bool m_bReleased;	// Mouse over selected or not

private:    
    enum button_state { MOUSE_OUT, MOUSE_OVER, CLICKED };
    
    //bool m_bReleased;
    
    int m_callbackID;
    
    void (*m_callback)();
};
/*
class MenuButtonCreator : public BaseCreator {
    GameObject* createGameObject() const {
        return new MenuButton();
    }
};
*/


#endif /* defined(__SDL_Game_Programming_Book__MenuObject__) */

