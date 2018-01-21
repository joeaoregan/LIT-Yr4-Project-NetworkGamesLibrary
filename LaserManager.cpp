
#include "LaserManager.h"
#include "Game.h"
#include "Audio.h"


LaserManager* LaserManager::s_pInstance = new LaserManager();

LaserManager::LaserManager()
{
}

void LaserManager::addLaser(int x, int y, int v) {
	Audio::Instance()->playFX("laserFX");
	Laser* pLaser = new Laser();
	pLaser->spawn(x,y,v);
	m_Lasers.push_back(pLaser);
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
