/*
	Joe O'Regan
	K00203642

	Player.cpp

	Functionality for the Player GameObject
	Handle movement
*/

#include "Player.h"
#include "Game.h"
#include "Texture.h"
#include "Input.h"
#include "LaserManager.h"

// Add gap between lasers firing
Uint32 lastFire;
Uint32 fireInterval = 500;

void Player::update(){  
	//std::cout << "Player update" << std::endl;

	handleInput();

	setX(getX() + getVelX());					// Move the Player left or right
   
	if((getX() < 0) || (getX() + getWidth() > SCREEN_WIDTH)) {	// If the Player went too far to the left or right  
		setX(getX() - getVelX());				// Move the Player back
	}
   
	setY(getY() + getVelY());					// Move the Player up

	if((getY() < 40) || (getY() + getHeight() > 600)) {  		// If the Player went too far up or down    
		setY(getY() - getVelY());				// Move the Player down
	}

}

void Player::handleInput(){
	// Vertical movement
	if (Input::Instance()->isKeyDown(SDL_SCANCODE_UP)) {
		setVelY(-getVel());
		//std::cout << "up" << std::endl;
	}  else if(Input::Instance()->isKeyDown(SDL_SCANCODE_DOWN)) {
		setVelY(getVel());
	//	std::cout << "down" << std::endl;
        } else {
		setVelY(0);
	}	 

        // Horizontal movement
        if(Input::Instance()->isKeyDown(SDL_SCANCODE_LEFT)) {
		setVelX(-getVel());
	//	std::cout << "left" << std::endl;
        } else if(Input::Instance()->isKeyDown(SDL_SCANCODE_RIGHT)) {
		setVelX(getVel());
	//	std::cout << "right" << std::endl;
        } else {
		setVelX(0);
	}
        
	// Fire weapons
        if(Input::Instance()->isKeyDown(SDL_SCANCODE_SPACE) && SDL_GetTicks() >= lastFire + fireInterval) {
		//PlayState::Instance()->spawnLaser();
		//LaserManager::Instance()->addLaser(getX() + 65, getY() + 30, 20);
		LaserManager::Instance()->addLaser(getX() + 65, getY() + 30, 20, getNetID());				// 30/01/2018 Added player ID to send string
		lastFire = SDL_GetTicks();
        }
}
/*
void Player::handleEvent( SDL_Event& e ) {
    //If a key was pressed
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ) {
        //Adjust the velocity
        switch( e.key.keysym.sym ) {
            case SDLK_UP: setVelY(-getVel()); break;
            case SDLK_DOWN: setVelY(getVel()); break;

            case SDLK_LEFT: setVelX(-getVel()); break;
            case SDLK_RIGHT: setVelX(getVel()); break;

	    case SDLK_SPACE: Game::Instance()->spawnLaser();
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 ) {
        // Adjust the velocity
        switch( e.key.keysym.sym ) {
            case SDLK_UP: setVelY(0); break;
            case SDLK_DOWN: setVelY(0); break;

            case SDLK_LEFT: setVelX(0); break;
            case SDLK_RIGHT: setVelX(0); break;
        }
    }
}
*/
void Player::render() {
	SDL_Rect renderQuad = { getX(), getY(), getWidth(), getHeight() };										// Set rendering space and render to screen

	SDL_RenderCopyEx(Game::Instance()->getRenderer(), Texture::Instance()->getTexture(getTextureID()), NULL, &renderQuad, 0, NULL, SDL_FLIP_NONE);	// Render to screen
};
