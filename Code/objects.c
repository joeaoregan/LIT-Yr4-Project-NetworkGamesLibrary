#include "objects.h"
#include "physic.h"
#include "Definitions.h"
#include "SDLFunctions.h"
#include "JOR_Net.h"				// MAX_PLAYERS definition

/*
	Initialise the list of players
	Moved from main.c
*/
void initPlayer(struct Player *players) {
	int i;
	for (i = 0; i < MAX_PLAYERS; i++) {
		players[i].position = makeRect(SPAWN_X, SPAWN_Y, PLAYER_WIDTH, PLAYER_HEIGHT);							// Init player position SDL_Rect
		players[i].left_key = SDLK_LEFT;
		players[i].right_key = SDLK_RIGHT;
		players[i].up_key = SDLK_UP;
		players[i].down_key = SDLK_DOWN;
		players[i].attack_key = SDLK_SPACE;																		// Change fire to spacebar (Was SDLK_z)
		players[i].face = 1;
		players[i].shoot = false;
		players[i].y_speed = 0;
		players[i].can_jump = false;
		players[i].reloading = false;
		players[i].kills = 0;
		players[i].deaths = 0;
		players[i].flip = 0;
	}
}

void resolve_player_key_down(int key, struct Player* player) {
    if (key == player->left_key) {
        player->left = true;
    }
    if (key == player->right_key) {
        player->right = true;
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
}

void resolve_player_key_up(int key, struct Player* player) {
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
}


void resolve_keyboard(SDL_Event e, struct Player* player) {
    if (e.type == SDL_KEYDOWN) {
        resolve_player_key_down(e.key.keysym.sym, player);
		//if (e.key.keysym.sym == SDLK_LEFT) player->flip = 1;			// Not needed, player->flip is now sent back from server
		//else if (e.key.keysym.sym == SDLK_RIGHT) player->flip = 0;
    } 
    if (e.type == SDL_KEYUP) {
        resolve_player_key_up(e.key.keysym.sym, player);
    }
}

void set_player_pos(struct Player* player, float x, float y) {
    player->position.x = (int) x;
    player->position.y = (int) y;
}

struct Bullet init_bullet(int x, int y, int face) {
    struct Bullet b;
	b.position = makeRect(x, y, BULLET_WIDTH, BULLET_HEIGHT);			// Init bullet position SDL_Rect
    b.face = face;
    return b;
}
