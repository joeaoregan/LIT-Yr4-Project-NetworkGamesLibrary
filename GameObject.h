/*
	Joe O'Regan
	K00203642

	GameObject.h

	Base class for game objects
*/
#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

#include <SDL2/SDL.h>				// SDL
//#include "Texture.h"
#include <iostream>

class GameObject {
public:
	GameObject() {};
	~GameObject() {};			// Deconstructor

	int getX() { return m_x; }		// Get GameObject X coord
	int getY() { return m_y; }		// Get GameObject Y coord
	int getVelX() { return m_xVel; }
	int getVelY() { return m_yVel; }
	int getVel() { return m_Vel; }
	int getWidth() { return m_width; }
	int getHeight() { return m_height; }
	bool getAlive() { return m_Alive; }

	void setX(int x) { m_x = x; }		// Set GameObject X coord
	void setY(int y) { m_y = y; }		// Set GameObject Y coord
	void setVelX(int x) { m_xVel = x; }
	void setVelY(int y) { m_yVel = y; }
	void setVel(int v) { m_Vel = v; }
	void setWidth(int w) { m_width = w; }
	void setHeight(int h) { m_height = h; }
	void setAlive(bool a) { m_Alive = a; }

	void spawn(int x, int y, int v) {	// Spawn at coords with velocity
		setX(x);
		setY(y);
		setVel(v);
	}

	void move() {};
	void render() {};

	//SDL_Rect getCollider();
	//void setColliderW(int w);
	//void setColliderH(int h);
	//void setColliderX(int x);
	//void setColliderY(int y);

private:
	// GameObject Variables
	int m_x, m_y;				// GameObject coordinates
	int m_xVel, m_yVel, m_Vel;
	int m_width, m_height;			// GameObject dimensions
	bool m_Alive;
	//SDL_Rect mCollider;
};

#endif

