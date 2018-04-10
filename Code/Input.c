/*
	Modified by:	Joe O'Regan
					K00203642

	Input.h

	Added  keyboard keypad input
*/

#include "Input.h"
#include "Definitions.h"

/*
	Handle input from keyboard keypad
*/
void keypadInput(int *key) {
	switch (*key) {														// JOR Keypad keys added
	case SDLK_KP_0: *key = SDLK_0; break;								// If keypad 0 key pressed, use 0 value
	case SDLK_KP_1: *key = SDLK_1; break;
	case SDLK_KP_2: *key = SDLK_2; break;
	case SDLK_KP_3: *key = SDLK_3; break;
	case SDLK_KP_4: *key = SDLK_4; break;
	case SDLK_KP_5: *key = SDLK_5; break;
	case SDLK_KP_6: *key = SDLK_6; break;
	case SDLK_KP_7: *key = SDLK_7; break;
	case SDLK_KP_8: *key = SDLK_8; break;
	case SDLK_KP_9: *key = SDLK_9; break;
	case SDLK_KP_PERIOD: *key = SDLK_PERIOD; break;						// JOR Keypad . was displaying strange character
	}
}

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
