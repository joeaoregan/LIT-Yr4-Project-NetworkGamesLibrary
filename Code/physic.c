/*
	Modified by:	Joe O'Regan
					K00203642

	Physics.c

	Added support for additional tiles to the tiled map
	Extra collision detection is handled for additional tiles
	Added sprite flipping for change in direction
*/

#include "physic.h"
#include "Definitions.h"

#define TILE_TYPES 6	// Anything over 6 tiles not checked for collisions

/*
	Added additional tiles to map, not just 1s and 0s
*/
int tileMap[15][20] = {
    {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,1},
    {6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,3,1,1,1,1,1,1,1,1,4,0,0,0,0,0},
    {0,0,0,0,0,0,0,5,2,2,2,2,6,0,0,0,0,0,0,0},
    {0,3,4,0,0,0,0,0,0,5,6,0,0,0,0,0,0,3,4,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,3,1,4,0,0,0,0,0,0,0,0,3,1,4,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,1,8,0,0,3,1,4,0,0,0,0,3,1,4,0,0,8,1,2},
    {2,2,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,2,2},
    {2,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,2,2},
    {2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2}};

int sign(int a) {
	return (a < 0) ? -1 : (a > 0) ? 1 : 0;	// JOR
}

void decrement_abs(int *a) {
    *a -= sign(*a);
}

/*
	Added check for collisions with multiple tiles
*/
int checkTileCollisions(SDL_Rect *rect) {
	int i;
	for (i = 1; i < TILE_TYPES; i++) {
		if ((tileMap[rect->y / TILE_SIZE][rect->x / TILE_SIZE] > 0 && 
				tileMap[rect->y / TILE_SIZE][rect->x / TILE_SIZE] < TILE_TYPES) ||
			(tileMap[(rect->y + rect->h) / TILE_SIZE][rect->x / TILE_SIZE] > 0 && 
				tileMap[(rect->y + rect->h) / TILE_SIZE][rect->x / TILE_SIZE] < TILE_TYPES) ||
			(tileMap[(rect->y) / TILE_SIZE][(rect->x + rect->w) / TILE_SIZE] > 0 &&
				tileMap[(rect->y) / TILE_SIZE][(rect->x + rect->w) / TILE_SIZE] < TILE_TYPES) ||
			(tileMap[(rect->y + rect->h) / TILE_SIZE][(rect->x + rect->w) / TILE_SIZE] > 0 && 
				tileMap[(rect->y + rect->h) / TILE_SIZE][(rect->x + rect->w) / TILE_SIZE] < TILE_TYPES) ||
			rect->x <= 0 || (rect->x + rect->w >= SCREEN_WIDTH)) {									// Left and right of screen

			//printf("Player/Map collision\n");
			return true;
		}
	}
	return false;
}

/*
	Move the player and check collisions with tiles
*/
int move_and_check_collisions(SDL_Rect *position, int axis, int mov) {
    SDL_Rect temp = *position;																		// Store position, and use temp var to check updated position

    if (axis == X_AXIS) { temp.x += sign(mov); }													// Check collision for intended new position on X axis
    if (axis == Y_AXIS) { temp.y += sign(mov); }													// Check intended position on Y axis

    if (checkTileCollisions(&temp)) {																// If there is a collision
		return 0; }																					// Don't update the position
	else {																							// Otherwise
        *position = temp;																			// Set the position to the new position
        return 1;
    }
}

/*
	Update the bullet object movement
*/
void updateBullets(struct node **bullets) {
    struct node *bulletList = *bullets;																// Get the bullet linked list
    Bullet *bullet = NULL;
    int i = 0;

    while (bulletList != NULL) {																	// Until the end of the linked list is reached
        bullet = (Bullet*) bulletList->data;														// Get the next bullet in the list
        bullet->position.x += BULLET_SPEED * bullet->face * 1;										// Move the bullet in the direction it is facing
        bulletList = bulletList->next;

        if (checkTileCollisions(&bullet->position)) {												// Check collisions with tiles
			/* JOR Save coordinates and indicate explosion at this point */
            erase_element(bullets, i);																// Clear the bullet if it collides with a tile
        } else {
            i++;
        }
    }
}

/*
	Handle collisions between players and bullets
	Using Axis Aligned Bounding Box collision detection
*/
int check_if_player_dies(Player *player, struct node **bullets, int *killer) {
    struct node *bulletList = *bullets;
    struct SDL_Rect boundingBoxBullet;																// Bullet bounding box
    struct SDL_Rect boundingBoxPlayer = player->position;											// Player bounding box
    int i = 0;

    while (bulletList != NULL) {																	// Until the end of the bullet linked list
        boundingBoxBullet = ((Bullet*) bulletList->data)->position;									// Get the bullet bounding box

        if (boundingBoxPlayer.x < (boundingBoxBullet.x + boundingBoxBullet.w) &&					// AABB collision check
                (boundingBoxPlayer.x + boundingBoxPlayer.w) > boundingBoxBullet.x &&
                boundingBoxPlayer.y < (boundingBoxBullet.y + boundingBoxBullet.h) &&
                (boundingBoxPlayer.y + boundingBoxPlayer.h) > boundingBoxBullet.y) {
            *killer = ((Bullet*) bulletList->data)->player_id;										// Identify the player who got the kill
            erase_element(bullets, i);																// Clear the bullet from the list of bullets

            return true;
        }

        bulletList = bulletList->next;																// Get the next bullet from the linked list
        i++;
    }

    return false;
}

/*
	Update the player objects movement (Added sprite flipping, when changing direction)
*/
void updatePlayer(Player *player) {
    int x_movement = 0;																				// Set/reset movement
    int y_movement = 0;
		
    if (player->left) {
        x_movement -= PLAYER_SPEED;
        player->face = BACKWARDS;																	// Player is facing backwards (Direction to shoot is right to left)
		player->flip = 1;
		//printf("test flip: %d\n", player->flip);
    }
	
    if (player->right) {
        x_movement += PLAYER_SPEED;
        player->face = FORWARDS;																	// Player is facing forwards (Direction to shoot is left to right)
		player->flip = 0;
		//printf("test flip: %d\n", player->flip);
    }
    if (player->up) {
        if (player->can_jump) {
            player->can_jump = false;
            player->y_speed = -PLAYER_JUMP_POWER;
        }
    }
    
    y_movement = player->y_speed / 3;

    if (player->y_speed < MAX_VERTICAL_SPEED) {
        player->y_speed += GRAVITY;
    }

    while (x_movement != 0 || y_movement != 0) {
        if (x_movement != 0 && move_and_check_collisions(&player->position, X_AXIS, x_movement)) {
            decrement_abs(&x_movement);
        } else {
            x_movement = 0;
        }

        if (y_movement != 0 && move_and_check_collisions(&player->position, Y_AXIS, y_movement)) {
            decrement_abs(&y_movement);
            player->can_jump = false;
        } else {
            if(y_movement > 0) {
                player->can_jump = true;
                player->y_speed = 0;
            }
            if(y_movement < 0) {
                player->y_speed = 0;
            }
            y_movement = 0;
        }
    }
}

/*
	Added additional tiles to the tile map
*/
SDL_Texture* get_map_texture(SDL_Renderer *renderer) {
	SDL_Rect rect = { 0, 0, TILE_SIZE, TILE_SIZE };													// Tile position and dimensions

	SDL_Surface *bmpTile1 = SDL_LoadBMP("Resources/tile1.bmp");
	SDL_Surface *bmpTile2 = SDL_LoadBMP("Resources/tile2.bmp");										// JOR Additional tiles added
	SDL_Surface *bmpTile3 = SDL_LoadBMP("Resources/tile3.bmp");
	SDL_Surface *bmpTile4 = SDL_LoadBMP("Resources/tile4.bmp");
	SDL_Surface *bmpTile5 = SDL_LoadBMP("Resources/tile5.bmp");

	SDL_Texture *texTile1 = SDL_CreateTextureFromSurface(renderer, bmpTile1);
	SDL_Texture *texTile2 = SDL_CreateTextureFromSurface(renderer, bmpTile2);
	SDL_Texture *texTile3 = SDL_CreateTextureFromSurface(renderer, bmpTile3);
	SDL_Texture *texTile4 = SDL_CreateTextureFromSurface(renderer, bmpTile4);
	SDL_Texture *texTile5 = SDL_CreateTextureFromSurface(renderer, bmpTile5);

	SDL_Texture *map_texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888, 
		SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderTarget(renderer, map_texture);

    int i, j;

	/*
		JOR: Additional tile layout checks
	*/
    for (i = 0; i < SCREEN_HEIGHT / TILE_SIZE; i++) {
        for (j = 0; j < SCREEN_WIDTH / TILE_SIZE; j++) {
			rect.x = TILE_SIZE * j;
			rect.y = TILE_SIZE * i;

			if (tileMap[i][j] == 1) 
				SDL_RenderCopy(renderer, texTile1, NULL, &rect);
			else if (tileMap[i][j] == 2) 
				SDL_RenderCopy(renderer, texTile2, NULL, &rect);
			else if (tileMap[i][j] == 3) 
				SDL_RenderCopyEx(renderer, texTile3, NULL, &rect, 0, NULL, 0);
			else if (tileMap[i][j] == 4) 
				SDL_RenderCopyEx(renderer, texTile3, NULL, &rect, 0, NULL, SDL_FLIP_HORIZONTAL);	// Reverse Tile 3
			else if (tileMap[i][j] == 5) 
				SDL_RenderCopyEx(renderer, texTile4, NULL, &rect, 0, NULL, 0);
			else if (tileMap[i][j] == 6) 
				SDL_RenderCopyEx(renderer, texTile4, NULL, &rect, 0, NULL, SDL_FLIP_HORIZONTAL);	// Reverse Tile 4
			else if (tileMap[i][j] == 7) 
				SDL_RenderCopyEx(renderer, texTile5, NULL, &rect, 0, NULL, 0);
			else if (tileMap[i][j] == 8) 
				SDL_RenderCopyEx(renderer, texTile5, NULL, &rect, 0, NULL, SDL_FLIP_VERTICAL);		// Vertically flip tile 5
        }
    }

    SDL_SetRenderTarget(renderer, NULL);
    return map_texture;
}
    
