#include "objects.h"
#include "physic.h"
#include "Definitions.h"
#include "SDLFunctions.h"

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
		if (e.key.keysym.sym == SDLK_LEFT) player->flip = 1;
		else if (e.key.keysym.sym == SDLK_RIGHT) player->flip = 0;
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
	b.position = makeRect(x, y, BULLET_WIDTH, BULLET_HEIGHT);
    //b.position.x = x;
    //b.position.y = y;
    //b.position.w = BULLET_WIDTH;
    //b.position.h = BULLET_HEIGHT;
    b.face = face;
    return b;
}
