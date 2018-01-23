
#include "LaserManager.h"
#include "Game.h"
#include "Audio.h"
//#include "Networking/Socket.h"
#include "Networking/NetJOR.h"

LaserManager* LaserManager::s_pInstance = new LaserManager();

LaserManager::LaserManager()
{
}

void LaserManager::addLaser(int x, int y, int v) {
//	Mix_PlayChannel( -1, laserFX, 0 );											// 20180120 Sound effects not playing now ???
//	std::cout << "sound" << std::endl;
/*	Audio::Instance()->playFX("laserFX");
	GameObject* p_Laser1 = new Laser();											// 20180120 Add laser to game object list 
	listOfGameObjects.push_back(p_Laser1);
	p_Laser1->spawn(player->getX() + 65, player->getY() + 30, 20);
*/
	Audio::Instance()->playFX("laserFX");
	Laser* pLaser = new Laser();
	pLaser->spawn(x,y,v);
	m_Lasers.push_back(pLaser);
	//sendToServer("1 Player_Laser_Fired");				// Error with header files
	NetJOR::Instance()->sendText("1 Player_Laser_Fired");		// Sends char* string to server
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
    for (int p = 0; p < m_Lasers.size(); p++) {
        m_Lasers[p]->render();
    }
}
