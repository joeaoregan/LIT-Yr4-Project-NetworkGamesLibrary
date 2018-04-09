#include "network.h"
#include "Definitions.h"

int16_t key_state_from_player(struct Player *player) {
	//printf("key_state from player\n");
    int16_t key_state = 0;
    if (player->left) {
        key_state = key_state | LEFT_KEY;
    }
    if (player->right) {
        key_state = key_state | RIGHT_KEY;
    }
    if (player->up) {
        key_state = key_state | UP_KEY;
    }
    if (player->down) {
        key_state = key_state | DOWN_KEY;
    }
    if (player->shoot) {
        key_state = key_state | ATTACK_KEY;
    }
    return key_state;
}

void player_from_key_state(struct Player *player, int16_t key_state) {	
	//printf("player from key state\n");
    if (key_state & LEFT_KEY) {
        player->left = true;
    } else {
        player->left = false;
    }
    if (key_state & RIGHT_KEY) {
        player->right = true;
    } else {
        player->right = false;
    }
    if (key_state & UP_KEY) {
        player->up = true;
    } else {
        player->up = false;
    }
    if (key_state & DOWN_KEY) {
        player->down = true;
    } else {
        player->down = false;
    }
    if (key_state & ATTACK_KEY) {
        player->shoot = true;
    } else {
        player->shoot = false;
    }
}
