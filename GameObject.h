/*
	Joe O'Regan
	K00203642

	GameObject.h

	Cross-platform base class for game objects
*/
#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

// Unix (Ubuntu)
#if defined __linux__
#include <SDL2/SDL.h>						// SDL
// Windows
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#endif

//#include "Texture.h"
#include <iostream>
#include "Game.h"
#include "Texture.h"

class GameObject {
public:
	GameObject() {};
	~GameObject() {};					// Deconstructor

	//virtual void handleEvent(SDL_Event& e) {}		// Handle events for objects

	virtual void update() {
		setX(getX() + getVel());
	};

	virtual void render() {
		//m_Texture.render(getX(), getY(), NULL, 0, NULL, SDL_FLIP_NONE);
		SDL_Rect renderQuad = { getX(), getY(), m_width, m_height };						// Set rendering space and render to screen

		/*
		if (clip != NULL) {											// Set clip rendering dimensions
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}
		*/
		//SDL_RenderCopyEx(Game::Instance()->getRenderer(), m_Texture, NULL, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);	// Render to screen
		SDL_RenderCopyEx(Game::Instance()->getRenderer(), Texture::Instance()->getTexture(getTextureID()), NULL, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);	// Render to screen
	};

	void renderAnimation() {
		//Texture::Instance()->modifyAlpha( getAlpha(), getTextureID() );
		SDL_Rect renderQuadSource = { getWidth() * getCurrentFrame(), getHeight() * getAnimRow(), getWidth(), getHeight() };
		SDL_Rect renderQuadDest = { getX(), getY(), getWidth(), getHeight() };

		SDL_RenderCopyEx(Game::Instance()->getRenderer(), Texture::Instance()->getTexture(getTextureID()), &renderQuadSource, &renderQuadDest, 0, NULL, SDL_FLIP_NONE);
	};

	virtual void clean() {}

	// Get & Set methods
	int getX() { return m_x; }				// Get GameObject X coord
	int getY() { return m_y; }				// Get GameObject Y coord
	int getVelX() { return m_xVel; }
	int getVelY() { return m_yVel; }
	int getVel() { return m_Vel; }
	int getWidth() { return m_width; }
	int getHeight() { return m_height; }
	bool getAlive() { return m_Alive; }
	std::string getTextureID() const { return m_TextureID; }// Return the texture ID
	int getCurrentFrame() const { return m_CurrentFrame; }	// The current animation frame of sprite sheet
	int getAnimRow() const { return m_CurrentAnimationRow; }// Get the current animation row of sprite sheet
	int getAlpha() const { return m_Alpha; }		// Get the texture alpha value

	void setX(int x) { m_x = x; }				// Set GameObject X coord
	void setY(int y) { m_y = y; }				// Set GameObject Y coord
	void setVelX(int x) { m_xVel = x; }
	void setVelY(int y) { m_yVel = y; }
	void setVel(int v) { m_Vel = v; }
	void setWidth(int w) { m_width = w; }
	void setHeight(int h) { m_height = h; }
	void setAlive(bool a) { m_Alive = a; }
	void setTextureID( std::string t) { 
		std::cout << "Texture set to: " << t << std::endl;
		m_TextureID = t; 
	}	// Set the texture ID
	void setCurrentFrame(int f) { m_CurrentFrame = f; }	// Set the current frame of animation in the sprite sheet
	void setAnimRow(int f) { m_CurrentAnimationRow = f; }	// Set the current row of animation in the sprite sheet
	void setAlpha(int a) { m_Alpha = a; }			// Set the texture alpha value

	void spawn(int x, int y, int v) {			// Spawn at coords with velocity
		setX(x);
		setY(y);
		setVel(v);
	}

	//SDL_Rect getCollider();
	//void setColliderW(int w);
	//void setColliderH(int h);
	//void setColliderX(int x);
	//void setColliderY(int y);
protected: 
	int m_currentFrame;					// Current animation frame

private:
	// GameObject Variables
	int m_x, m_y;						// GameObject coordinates
	int m_xVel, m_yVel, m_Vel;
	int m_width, m_height;					// GameObject dimensions
	bool m_Alive;
	//SDL_Rect mCollider;
	SDL_Texture* m_Texture;					// The actual hardware texture
	std::string m_TextureID;				// ID for the texture associated with the object
	int m_CurrentFrame;					// The current animation frame in the sprite sheet
	int m_CurrentAnimationRow;				// The current animation row in the sprite sheet
	int m_Alpha;						// Alpha value for Texture
};

#endif

