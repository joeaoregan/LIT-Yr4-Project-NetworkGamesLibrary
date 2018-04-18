/*
	Modified by:	Joe O'Regan
					K00203642

	GameObject.c

	Added change in direction for player sprite
	Communicated over network, so player flips
	direction on all clients
*/

#include "GameObject.h"																	// Game objects Player and Bullet
#include "physic.h"
#include "Definitions.h"
#include "SDLFunctions.h"
#include "JOR_Net.h"																	// JN_MAX_PLAYERS definition

/*
	Initialise the list of players
	Moved from main.c
*/
void initPlayer(Player *players) {
	unsigned int i;
	for (i = 0; i < JN_MAX_PLAYERS; i++) {
		players[i].position = makeRect(SPAWN_X, SPAWN_Y, PLAYER_WIDTH, PLAYER_HEIGHT);	// Init player position SDL_Rect
		players[i].left_key = SDLK_LEFT;
		players[i].right_key = SDLK_RIGHT;
		players[i].up_key = SDLK_UP;
		players[i].down_key = SDLK_DOWN;
		players[i].attack_key = SDLK_SPACE;												// Change fire to spacebar (Was SDLK_z)
		players[i].face = 1;
		players[i].shoot = false;
		players[i].y_speed = 0;
		players[i].can_jump = false;
		players[i].reloading = false;
		players[i].kills = 0;
		players[i].deaths = 0;
		players[i].flip = 0;
	}

	JOR_NetTextColour("Player List initilised\n", GREEN);
}

/*
	Set the player key presses to send to the server
*/
bool isKeyDown(int key, Player* player) {
    if (key == player->left_key) {
        player->left = true;
		//printf("LEFT LEFT LEFT LEFT LEFT LEFT\n");
    }
    if (key == player->right_key) {
        player->right = true;
		//printf("RIGHT RIGHT RIGHT RIGHT RIGHT \n");
    }
    if (key == player->up_key) {
        player->up = true;
    }
    if (key == player->down_key) {
        player->down = true;
    }
    if (key == player->attack_key) {
        player->shoot = player->face;
    }

	return (player->left || player->right || player->up || player->down || player->shoot);
}

/*
	If the key is no longer pressed
	Set the player stored key press to false
*/
bool isKeyUp(int key, Player* player) {
    if (key == player->left_key) {
        player->left = false;
    }
    if (key == player->right_key) {
        player->right = false;
    }
    if (key == player->up_key) {
        player->up = false;
    }
    if (key == player->down_key) {
        player->down = false;
    }
    if (key == player->attack_key) {
        player->shoot = false;
    }


	//return ((player->left && player->right && player->up && player->down && player->shoot));	// Moves around
	return (!(player->left && player->right && player->up && player->down && player->shoot));
}

/*
	Set the player key presses to the keyboard input
*/
bool getInputFromPlayer(SDL_Event e, Player* player) {
	bool keyPressed = false;
	//bool keyReleased = false;

    if (e.type == SDL_KEYDOWN) {
        keyPressed = isKeyDown(e.key.keysym.sym, player);
		//if (e.key.keysym.sym == SDLK_LEFT) player->flip = 1;							// Not needed, player->flip is now sent back from server
		//else if (e.key.keysym.sym == SDLK_RIGHT) player->flip = 0;
    } 
   // else 
	if (e.type == SDL_KEYUP) {
		keyPressed = isKeyUp(e.key.keysym.sym, player);
    }

	return keyPressed;
}
/*
// Unused Player position update function
void set_player_pos(Player* player, float x, float y) {
    player->position.x = (int) x;
    player->position.y = (int) y;
}
*/

/*
	Initialise the bullet using the player that fired its position as the spawn point
	and the direction they are facing as the direction to move in
*/
Bullet init_bullet(int x, int y, int face) {
	if (face != FORWARDS || face != BACKWARDS)											// JOR If the bullet isn't facing forwards or backwards it won't move
		face = FORWARDS;																// JOR Set default to forwards initially when player was spawned face was 0

    Bullet bullet;																		// Bullet object
	bullet.position = makeRect(x, y, BULLET_WIDTH, BULLET_HEIGHT);						// JOR Init bullet position SDL_Rect
    bullet.face = face;																	// Set the direction to aim the bullet

    return bullet;																		// Return the initiliased bullet object
}
