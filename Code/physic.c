#include "physic.h"
#include "Definitions.h"

#define TILE_TYPES 6

int map[15][20] = {
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
    if (a < 0) { return -1;  } 
	else if (a > 0) { return 1; } 
	else { return 0; }
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
		if ((map[rect->y / TILE_SIZE][rect->x / TILE_SIZE] > 0 && 
				map[rect->y / TILE_SIZE][rect->x / TILE_SIZE] < TILE_TYPES) ||
			(map[(rect->y + rect->h) / TILE_SIZE][rect->x / TILE_SIZE] > 0 && 
				map[(rect->y + rect->h) / TILE_SIZE][rect->x / TILE_SIZE] < TILE_TYPES) ||
			(map[(rect->y) / TILE_SIZE][(rect->x + rect->w) / TILE_SIZE] > 0 &&
				map[(rect->y) / TILE_SIZE][(rect->x + rect->w) / TILE_SIZE] < TILE_TYPES) ||
			(map[(rect->y + rect->h) / TILE_SIZE][(rect->x + rect->w) / TILE_SIZE] > 0 && 
				map[(rect->y + rect->h) / TILE_SIZE][(rect->x + rect->w) / TILE_SIZE] < TILE_TYPES) ||
			rect->x <= 0 || (rect->x + rect->w >= SCREEN_WIDTH)) {								// Left and right of screen

			//printf("Player/Map collision\n");
			return true;
		}
		else {
			return false;
		}
	}
}


int move_and_check_collisions(SDL_Rect *position, int axis, int mov) {
    SDL_Rect temp = *position;

    if (axis == X_AXIS) { temp.x += sign(mov); }
    if (axis == Y_AXIS) { temp.y += sign(mov); }

    if (checkTileCollisions(&temp)) { 
		return 0; } 
	else {
        *position = temp;
        return 1;
    }
}

/*
	Update the bullet object movement
*/
void updateBullets(struct node **bullets) {
    struct node *next = *bullets;
    struct Bullet *b = NULL;
    int i = 0;

    while (next != NULL) {
        b = (struct Bullet*) next->data;
        b->position.x += BULLET_SPEED * b->face * 1;
        next = next->next;
        if (checkTileCollisions(&b->position)) {
            erase_element(bullets, i);
        } else {
            i++;
        }
    }
}

int check_if_player_dies(struct Player *player, struct node **bullets, int *killer) {
    struct node *next = *bullets;
    struct SDL_Rect tmpBullet; 
    struct SDL_Rect tmpPlayer = player->position;
    int i = 0;
    while (next != NULL) {
        tmpBullet = ((struct Bullet*) next->data)->position;
        if (tmpPlayer.x < (tmpBullet.x + tmpBullet.w) &&
                (tmpPlayer.x + tmpPlayer.w) > tmpBullet.x &&
                tmpPlayer.y < (tmpBullet.y + tmpBullet.h) &&
                (tmpPlayer.y + tmpPlayer.h) > tmpBullet.y) {
            *killer = ((struct Bullet*) next->data)->player_id;			// Identify the player who got the kill
            erase_element(bullets, i);									// Clear the bullet from the list of bullets
            return true;
        }
        next = next->next;
        i++;
    }
    return false;
}

/*
	Update the player objects movement
*/
void updatePlayer(struct Player *player) {
    int x_movement = 0;
    int y_movement = 0;

    if (player->left) {
        x_movement -= PLAYER_SPEED;
        player->face = -1;
		player->flip = 1;
		//printf("test flip: %d\n", player->flip);
    }
    if (player->right) {
        x_movement += PLAYER_SPEED;
        player->face = 1;
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
	Added additional tiles
*/
SDL_Texture* get_map_texture(SDL_Renderer *renderer) {
	SDL_Rect rect = { 0, 0, TILE_SIZE, TILE_SIZE };														// Tile position and dimensions
	SDL_Surface *bmpTile1 = SDL_LoadBMP("../resources/tile1.bmp");
	SDL_Surface *bmpTile2 = SDL_LoadBMP("../resources/tile2.bmp");										// Additional tiles
	SDL_Surface *bmpTile3 = SDL_LoadBMP("../resources/tile3.bmp");
	SDL_Surface *bmpTile4 = SDL_LoadBMP("../resources/tile4.bmp");
	SDL_Surface *bmpTile5 = SDL_LoadBMP("../resources/tile5.bmp");
	SDL_Texture *texTile1 = SDL_CreateTextureFromSurface(renderer, bmpTile1);
	SDL_Texture *texTile2 = SDL_CreateTextureFromSurface(renderer, bmpTile2);
	SDL_Texture *texTile3 = SDL_CreateTextureFromSurface(renderer, bmpTile3);
	SDL_Texture *texTile4 = SDL_CreateTextureFromSurface(renderer, bmpTile4);
	SDL_Texture *texTile5 = SDL_CreateTextureFromSurface(renderer, bmpTile5);
	SDL_Texture *map_texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888, 
		SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderTarget(renderer, map_texture);

    int i, j, k;
    for (i = 0; i < SCREEN_HEIGHT / TILE_SIZE; i++) {
        for (j = 0; j < SCREEN_WIDTH / TILE_SIZE; j++) {
			rect.x = TILE_SIZE * j;
			rect.y = TILE_SIZE * i;

			for (k = 1; k <= TILE_TYPES; k++) {															// Render the different tile types
				if (map[i][j] == 1) SDL_RenderCopy(renderer, texTile1, NULL, &rect);
				else if (map[i][j] == 2) SDL_RenderCopy(renderer, texTile2, NULL, &rect);
				else if (map[i][j] == 3) SDL_RenderCopyEx(renderer, texTile3, NULL, &rect, 0, NULL, 0);
				else if (map[i][j] == 4) SDL_RenderCopyEx(renderer, texTile3, NULL, &rect, 0, NULL, SDL_FLIP_HORIZONTAL);	// Reverse Tile 3
				else if (map[i][j] == 5) SDL_RenderCopyEx(renderer, texTile4, NULL, &rect, 0, NULL, 0);
				else if (map[i][j] == 6) SDL_RenderCopyEx(renderer, texTile4, NULL, &rect, 0, NULL, SDL_FLIP_HORIZONTAL);	// Reverse Tile 4
				else if (map[i][j] == 7) SDL_RenderCopyEx(renderer, texTile5, NULL, &rect, 0, NULL, 0);
				else if (map[i][j] == 8) SDL_RenderCopyEx(renderer, texTile5, NULL, &rect, 0, NULL, SDL_FLIP_VERTICAL);		// Vertically flip tile 5
			}
        }
    }
    SDL_SetRenderTarget(renderer, NULL);
    return map_texture;
}
    
