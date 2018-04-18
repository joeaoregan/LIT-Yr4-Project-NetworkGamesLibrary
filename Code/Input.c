/*
	Modified by:	Joe O'Regan
					K00203642

	Input.c

	Added  keyboard keypad input
*/

#include "Input.h"
#include "Definitions.h"

/*
	JOR - Handle input from keyboard keypad
*/
void keypadInput(int *key) {
	switch (*key) {												// JOR Keypad keys support added
	case SDLK_KP_0: *key = SDLK_0; break;						// If keypad 0 key pressed, use 0 value
	case SDLK_KP_1: *key = SDLK_1; break;
	case SDLK_KP_2: *key = SDLK_2; break;
	case SDLK_KP_3: *key = SDLK_3; break;
	case SDLK_KP_4: *key = SDLK_4; break;
	case SDLK_KP_5: *key = SDLK_5; break;
	case SDLK_KP_6: *key = SDLK_6; break;
	case SDLK_KP_7: *key = SDLK_7; break;
	case SDLK_KP_8: *key = SDLK_8; break;
	case SDLK_KP_9: *key = SDLK_9; break;
	case SDLK_KP_PERIOD: *key = SDLK_PERIOD; break;				// JOR Keypad . was displaying strange character
	}
}

/*
	Apply binary OR to keystate if key is pressed
*/
int16_t getPlayerKeyState(Player *player) {
	//printf("key_state from player\n");
    int16_t key_state = 0;
    if (player->left) {
        key_state = key_state | LEFT_KEY;						// 00000000 or 00000001
    }
    if (player->right) {
        key_state = key_state | RIGHT_KEY;						// 00000000 or 00000010
    }
    if (player->up) {
        key_state = key_state | UP_KEY;							// 00000000 or 00000100
    }
    if (player->down) {
        key_state = key_state | DOWN_KEY;						// 00000000 or 00001000
    }
    if (player->shoot) {
        key_state = key_state | ATTACK_KEY;						// 00000000 or 00010000
    }
    return key_state;
}

/*
	Use binary AND to set Player key presses
*/
void setPlayerKeyPresses(Player *player, int16_t key_state) {	
	//printf("player from key state\n");
    if (key_state & LEFT_KEY) {									// If they are both 1 in first position 00000001 the left key is pressed
        player->left = true;
    } else {
        player->left = false;									// Otherwise it isn't
    }
    if (key_state & RIGHT_KEY) {								// etc.
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