/*
	Joe O'Regan
	K00203642

	LaserManager.cpp 

	Manage lasers
*/

#include "LaserManager.h"
#include "Game.h"
#include "Audio.h"

#ifdef	__NETWORKING_JOE_O_REGAN					// Check for Windows version of game that Network Library is present
#include "../Networking/NetJOR.h"
#endif

LaserManager* LaserManager::s_pInstance = new LaserManager();

LaserManager::LaserManager()
{
}

void LaserManager::addLaser(int x, int y, int v, int id) {
//	Mix_PlayChannel( -1, laserFX, 0 );											// 20180120 Sound effects not playing now ???
	std::cout << "Laser Fired" << std::endl;

	Audio::Instance()->playFX("laserFX");

	Laser* pLaser = new Laser();
	pLaser->spawn(x,y,v);
	m_Lasers.push_back(pLaser);

#ifdef	__NETWORKING_JOE_O_REGAN												// Check for Windows version of game that Network Library is present
	char msg[100];
	snprintf(msg, 40, "%d Player_Laser_Fired",id);

	//NetJOR::Instance()->sendText("1 Player_Laser_Fired");									// Sends char* string to server
	//NetJOR::Instance()->sendText(msg);											// Sends char* string to server
	NetJOR::Instance()->sendString(msg);
#endif	
}

void LaserManager::clear() {
    m_Lasers.clear();
}

void LaserManager::update() {
    for (std::vector<Laser*>::iterator p_it = m_Lasers.begin(); p_it != m_Lasers.end();) {
        if((*p_it)->getX() < 0 || (*p_it)->getX() > SCREEN_WIDTH  || (*p_it)->getY() < 0 || (*p_it)->getY() > SCREEN_HEIGHT) {
            delete * p_it;
            p_it = m_Lasers.erase(p_it);
        }
        else {
            (*p_it)->update();
            ++p_it;
        }
    }
}

void LaserManager::render() {
    for (unsigned int p = 0; p < m_Lasers.size(); p++) {
        m_Lasers[p]->render();
    }
}
