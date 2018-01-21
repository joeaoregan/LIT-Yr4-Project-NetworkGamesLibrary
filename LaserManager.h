#ifndef LASER_MANAGER_H
#define LASER_MANAGER_H

#include <iostream>
#include <vector>
#include "Laser.h"

class LaserManager {
public:    
	static LaserManager* Instance() {
		if(s_pInstance == 0) {
			s_pInstance = new LaserManager();
			return s_pInstance;
		}
		return s_pInstance;
	}
    
	void addLaser(int x, int y, int v);			// X position, Y position, and Velocity
    
	void update();
	void render();
    
	void clear();
    
	const std::vector<Laser*> getLasers() {return m_Lasers;}
    
private:    
	LaserManager();
	~LaserManager();
    
    	LaserManager(const LaserManager&);
    	LaserManager& operator=(const LaserManager&);
    
    	static LaserManager* s_pInstance;

	std::vector<Laser*> m_Lasers;    
};

#endif

