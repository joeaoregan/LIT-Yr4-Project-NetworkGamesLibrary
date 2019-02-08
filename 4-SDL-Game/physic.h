/*
	Modified by:	Joe O'Regan
					K00203642

	Originally by:	Maciej Spychala

	Physics.h

	Added support for additional tiles to the tiled map
	Extra collision detection is handled for additional tiles
	Added sprite flipping for change in direction
*/

#ifndef PHYSIC_H
#define PHYSIC_H

#include <SDL.h>
#include "GameObject.h"																		// Game objects Player and Bullet
#include "list.h"

void updatePlayer(Player *player);
SDL_Texture* get_map_texture(SDL_Renderer *renderer);
void updateBullets(struct node **bullets);
int check_if_player_dies(Player *player, struct node **bullets, int *killer);

#endif
