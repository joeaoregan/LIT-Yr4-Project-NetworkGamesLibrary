#include "PlayState.h"
#include "../Texture.h"							// Texture functions
#include "../Player.h"													// Player functions
#include <list>										// Store game objects in a list
#include <sstream>									// 20180117 Updating text
#include "../Background.h"							// 20180121

std::stringstream updateText;
std::vector<GameObject*> listOfGameObjects;				// List of game objects

GameObject* background;	 						// Scrolling background
GameObject* player;							// The Player that will be moving around on the screen	

// UI textures
Texture gTextTexture;

bool PlayState::onEnter() {
	std::cout << "Loading Play State" << std::endl;
	bool success = true;

	prevX = -1;
	prevY = -1;

	success = Texture::Instance()->loadTextures();									// Load the game textures

	std::cout << "Textures Loaded" << std::endl;


//	gFont = TTF_OpenFont( "Assets/Fonts/lazy.ttf", 28 );
	if( Game::Instance()->getFont() == NULL ) {
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	} else {
		//Render text
		SDL_Color textColor = { 255, 0, 0 };
		if( !gTextTexture.loadFromRenderedText( "Linux SDL Test Game - Joe O'Regan", textColor )) {
			printf( "Failed to render text texture!\n" );
			success = false;
		}
	}

	std::cout << "Font OK" << std::endl;

	// Create player and background, and add to game object list
	background = new Background();
	player = new Player();
	listOfGameObjects.push_back(background);
	listOfGameObjects.push_back(player);

	player->spawn(0,(SCREEN_HEIGHT - player->getHeight() - 120)/2, player->getVel());				// Center of play area
	std::cout << "Spawn Player" << std::endl;

	return success;
}

void PlayState::update(){		
	//std::cout << "PlayState update()" << std::endl;
	//player->handleEvent( e );											// Handle input for the Player
	//player->handleEvent( Game::Instance()->getEvent() );											// Handle input for the Player

	for (int index = 0; index != listOfGameObjects.size(); ++index) {	
		listOfGameObjects[index]->update();									// Move/Update the game objects
	}

	updateText.str("");													// 20180117			
	updateText << "0 Player1 " << player->getX() << " " << player->getY();							// 20180118 Send name/ID, x coord, y coord  -  to server

	if (player->getX() != prevX || player->getY() != prevY) {								// Only send update if position changes
		//sendToServer2(updateText.str().c_str());
		prevX = player->getX();
		prevY = player->getY();
	}
}

void PlayState::render() {
	for (int index = 0; index != listOfGameObjects.size(); ++index) {	
		listOfGameObjects[index]->render();										// Render the game object
	}

	gTextTexture.render((SCREEN_WIDTH - gTextTexture.getWidth()) / 2,((SCREEN_HEIGHT -600 -gTextTexture.getHeight())/ 2)+ 600);
}

bool PlayState::onExit() {
	std::cout << "Exit Play State" << std::endl;

	gTextTexture.free();

	return true;
}

// spawnlasers
