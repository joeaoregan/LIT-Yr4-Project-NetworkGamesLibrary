/*
	Joe O'Regan
	K00203642

	GameObject.cpp

	Cross-platform base class for game objects
*/

#include "GameObject.h"

void GameObject::update() {
	setX(getX() + getVel());
};

void GameObject::render() {
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

void GameObject::renderAnimation() {
	//Texture::Instance()->modifyAlpha( getAlpha(), getTextureID() );
	SDL_Rect renderQuadSource = { getWidth() * getCurrentFrame(), getHeight() * getAnimRow(), getWidth(), getHeight() };
	SDL_Rect renderQuadDest = { getX(), getY(), getWidth(), getHeight() };

	SDL_RenderCopyEx(Game::Instance()->getRenderer(), Texture::Instance()->getTexture(getTextureID()), &renderQuadSource, &renderQuadDest, 0, NULL, SDL_FLIP_NONE);
};

// Spawn object at coordinates with specified velocity
void GameObject::spawn(int x, int y, int v) {
	setX(x);
	setY(y);
	setVel(v);
}
