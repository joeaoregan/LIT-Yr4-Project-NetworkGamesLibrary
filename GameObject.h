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
#include <SDL2/SDL.h>							// SDL Windows
// Windows
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>							// SDL Linux
#endif

//#include "Texture.h"
#include <iostream>
#include "Game.h"
#include "Texture.h"

class GameObject {
public:
	GameObject() {};						// Constructor
	~GameObject() {};						// Deconstructor

	//virtual void handleEvent(SDL_Event& e) {}			// Handle events for objects

	virtual void update();						// Update/move the game object
	virtual void render();						// Draw the game object to screen
	virtual void clean() {}						// Clean up the game object when destroyed

	void renderAnimation();						// Draw animated objects to screen
	void spawn(int x, int y, int v);				// Spawn at coords with specified velocity
	
	// Get & Set methods
	int getX() { return m_x; }					// Get GameObject X coord
	int getY() { return m_y; }					// Get GameObject Y coord
	int getVelX() { return m_xVel; }				// Get velocity on X axis
	int getVelY() { return m_yVel; }				// Get velocity on Y axis
	int getVel() { return m_Vel; }					// Get velocity
	int getWidth() { return m_width; }				// Get object width
	int getHeight() { return m_height; }				// Get object height
	bool getAlive() { return m_Alive; }				// Is the object active in the scene
	std::string getTextureID() const { return m_TextureID; }	// Return the texture ID
	int getCurrentFrame() const { return m_CurrentFrame; }		// The current animation frame of sprite sheet
	int getAnimRow() const { return m_CurrentAnimationRow; }	// Get the current animation row of sprite sheet
	int getAlpha() const { return m_Alpha; }			// Get the texture alpha value
	std::string getName() const  { return m_Name; }

	void setX(int x) { m_x = x; }					// Set GameObject X coord
	void setY(int y) { m_y = y; }					// Set GameObject Y coord
	void setVelX(int x) { m_xVel = x; }				// Set velocity on X axis
	void setVelY(int y) { m_yVel = y; }				// Set velocity on Y axis
	void setVel(int v) { m_Vel = v; }				// Set velocity
	void setWidth(int w) { m_width = w; }				// Set width
	void setHeight(int h) { m_height = h; }				// Set height
	void setAlive(bool a) { m_Alive = a; }				// Set the object active
	void setTextureID( std::string t) { 				// Set texture to render
		std::cout << "Texture set to: " << t << std::endl;
		m_TextureID = t; 
	}
	void setCurrentFrame(int f) { m_CurrentFrame = f; }		// Set the current frame of animation in the sprite sheet
	void setAnimRow(int f) { m_CurrentAnimationRow = f; }		// Set the current row of animation in the sprite sheet
	void setAlpha(int a) { m_Alpha = a; }				// Set the texture alpha value
	void setName(std::string set) { m_Name = set; }			// Set the name for the object

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
	int m_xVel, m_yVel, m_Vel;				// Object velocity
	int m_width, m_height;					// GameObject dimensions
	bool m_Alive;						// Is the object active in the scene
	//SDL_Rect mCollider;
	SDL_Texture* m_Texture;					// The actual hardware texture
	std::string m_TextureID;				// ID for the texture associated with the object
	std::string m_Name;					// Name for object, to be used for debugging/networking
	int m_CurrentFrame;					// The current animation frame in the sprite sheet
	int m_CurrentAnimationRow;				// The current animation row in the sprite sheet
	int m_Alpha;						// Alpha value for Texture
};

#endif

