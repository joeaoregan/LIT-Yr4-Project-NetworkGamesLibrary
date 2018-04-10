/*
	Modified by:	Joe O'Regan
					K00203642

	GameObject.h

	Added change in direction for player sprite
	Communicated over network, so player flips
	direction on all clients
*/

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <SDL.h>

struct Player {
    SDL_Rect position;											// Position and dimensions
    SDL_Texture *texture;										// Player sprite
    int left, right, up, down;
    int can_jump, y_speed;										// Player is positioned on an object to jump off, the players speed
    int face, shoot, reloading;									// Direction the player is facing, the player has fired, and player is reloading weapon (sets fire rate)
    int left_key, right_key, up_key, down_key, attack_key;		// Keyboard key for movement in each direction and firing
    int kills, deaths, flip;									// Change the sprite direction when movement direction changes
};

struct Bullet {
    SDL_Rect position;											// Position and dimensions
    int face;													// Direction the bullet is to move in
    int player_id;												// Identifies the player who fired the bullet
};

void initPlayer(struct Player *players);						// Initialise the player struct

void resolve_player_key_up(int key, struct Player* player);
void resolve_player_key_down(int key, struct Player* player);
void resolve_keyboard(SDL_Event e, struct Player* player);
void set_player_pos(struct Player* player, float x, float y);
struct Bullet init_bullet(int x, int y, int face);

#endif

