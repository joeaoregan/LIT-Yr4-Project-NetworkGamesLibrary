#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_thread.h>	// JOR SDL Thread in place of pthread
#include <time.h>
#include <stdint.h>
#include "objects.h"
#include "client_udp.h"
#include "server_udp.h"
#include "network.h"
#include "physic.h"
#include "Definitions.h"
#include "font.h"
#include "menu.h"

struct Player players[MAX_PLAYERS];
int number_of_players = 0;
int16_t clientID = -1;
int16_t bullets_client[256];
int bullets_number = 0;

SDL_Texture* load_texture(SDL_Renderer *renderer, char *file) {
    SDL_Surface *bitmap = NULL;
    SDL_Texture *texture = NULL;
    bitmap = SDL_LoadBMP(file);
    texture = SDL_CreateTextureFromSurface(renderer, bitmap);
    SDL_FreeSurface(bitmap);
    return texture;
}

void init_players() {
    int i;
    for (i = 0; i < MAX_PLAYERS; i++) {
        players[i].position.x = SPAWN_X;
        players[i].position.y = SPAWN_Y;
        players[i].position.w = PLAYER_WIDTH;
        players[i].position.h = PLAYER_HEIGHT;
        players[i].left_key = SDLK_LEFT;
        players[i].right_key = SDLK_RIGHT;
        players[i].up_key = SDLK_UP;
        players[i].down_key = SDLK_DOWN;
        players[i].attack_key = SDLK_z;
        players[i].face = 1;
        players[i].shoot = false;
        players[i].y_speed = 0;
        players[i].can_jump = false;
        players[i].reloading = false;
        players[i].kills = 0;
        players[i].deaths = 0;
    }
}

void receive_new_id(int id) {
    clientID = id;
    number_of_players = id;
	printf("Client ID is now: %d\n", clientID);
}

void check_if_its_new_player(int id){
    if (id > number_of_players) {
        number_of_players = id;
		printf("Maximum number of playser is now %d\n", number_of_players + 1);
    }
}

//void* client_loop(void *arg) {
int client_loop(void *arg) {
    int socket = *((int *) arg);
    int16_t tab[BUF_MAX];
    int strLen;
    int id, bullets_in_array;
    while (1) {
        strLen = cliRecvfrom(socket, tab);

        id = tab[0];							// first int = id
        if (id == -1) {
            receive_new_id(tab[1]);				// Set an id for the connected player
        }

		printf("got this far - before id > 0 ID: %d\n", id);

        if (id >= 0) {
			printf("got this far - id > 0 ID: %d\n", id);
            check_if_its_new_player(id);		// Increase number of players if new player added
            players[id].position.x = tab[1];	// Player x position
            players[id].position.y = tab[2];	// Player y position
            players[id].kills = tab[3];			// Number of kills
            players[id].deaths = tab[4];		// Number of times died
        }

        if (id == -2) {
            bullets_in_array = (strLen - sizeof(int16_t)) / (sizeof(int16_t) * 2);
            memcpy(bullets_client, tab + 1, sizeof(int16_t) * 2 * bullets_in_array);
            bullets_number = bullets_in_array;
        }
		
#if defined __linux__
		usleep(50);
#elif defined _WIN32 || defined _WIN64
		Sleep(5/1000);
#endif

    }
	return 0;	// Changed function return type to remove incompatible pointer type warning
}

int main(int argc, char* argv[]) {							// Add formal parameter list
    struct sockaddr_in srvAddr, cliAddr;
    int srvSock, cliSock;
    char *server_ip_addr = NULL;

#if defined _WIN32 || defined _WIN64
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {				// If winsock doesn't initialise
		printf("Failed. Error Code : %d", WSAGetLastError());	// Display an error message
		exit(EXIT_FAILURE);										// And exit with the specified error code
	}
	printf("Initialised Winsock.\n");							// Otherwise indicate winsock has initialised
#endif

    char menu = 's';
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Texture *tex = NULL;
    SDL_Texture *bullet = NULL;
    SDL_Texture *map = NULL;
    TTF_Init();
    TTF_Font *font;
    font = TTF_OpenFont("../resources/m5x7.ttf", 24);
    init_players();
    window = SDL_CreateWindow( "SDL UDP Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);	// JOR Changed window title, added screen_width & screen_height

    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("Could not create renderer: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    map = get_map_texture(renderer);
    tex = load_texture(renderer, "../resources/player.bmp");
    bullet = load_texture(renderer, "../resources/bullet.bmp");
    int i;
    server_or_client(renderer, &menu, font);
    if (menu == 'c') {
        server_ip_addr = malloc(16 * sizeof(char));
        ask_for_ip(renderer, font, server_ip_addr);
    }

	SDL_Thread* thread_id_server = NULL;				// JOR SDL threads replacing pthread
	SDL_Thread* thread_id_client = NULL;
	SDL_Thread* thread_id_server_send = NULL;

    srvAddr = server_sock_addr(server_ip_addr);
    cliAddr = client_sock_addr();
    if(menu == 's') {
        prepare_server(&srvSock, &srvAddr);
		thread_id_server = SDL_CreateThread(server_receive_loop, "ServerReceiveThread", &srvSock);	// JOR SDL Thread replaces Pthread
		thread_id_server_send = SDL_CreateThread(server_send_loop, "ServerSendThread", &srvSock);
    }
    initClientUDPSock(&cliSock, &cliAddr);
	thread_id_client = SDL_CreateThread(client_loop, "ClientThread", &cliSock);						// JOR SDL Thread replaces Pthread

    while (clientID < 0) {
		//printf("This one -> ");
        srvSendTo(cliSock, srvAddr, clientID, 0);
		
#if defined __linux__
		usleep(100);
#elif defined _WIN32 || defined _WIN64
		Sleep(10/1000);
#endif

    }

    SDL_Rect bullet_pos;
    bullet_pos.w = BULLET_HEIGHT;
    bullet_pos.h = BULLET_HEIGHT;

	SDL_Event e;
	int quit = 0;

    while (quit == 0) {
        if (SDL_PollEvent(&e)) {
            //if (e.type == SDL_QUIT) { break; }
            if (e.type == SDL_QUIT) { quit = 1; }

            resolve_keyboard(e, &players[clientID]);
        }
        srvSendTo(cliSock, srvAddr, clientID, key_state_from_player(&players[clientID]));
		
#if defined __linux__
		usleep(30);
#elif defined _WIN32 || defined _WIN64
		Sleep(3/1000);
#endif

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, map, NULL, NULL);

        for (i = 0; i <= number_of_players; i++) {
            SDL_RenderCopy(renderer, tex, NULL, &players[i].position);
        }

        disp_text(renderer, "Kills:", font, 400, 10);

        for (i = 0; i <= number_of_players; i++) {
            char kills[10] = "";
            snprintf(kills, 3, "%d", players[i].kills);
            disp_text(renderer, kills, font, 400, 30 + i * 20);
        }

        disp_text(renderer, "Deaths:", font, 460, 10);

        for (i = 0; i <= number_of_players; i++) {
            char deaths[10] = "";
            snprintf(deaths, 3, "%d", players[i].deaths);
            disp_text(renderer, deaths, font, 460, 30 + i * 20);
        }

        for (i = 0; i < bullets_number; i++) {
            bullet_pos.x = bullets_client[i*2];
            bullet_pos.y = bullets_client[i*2 + 1];
            SDL_RenderCopy(renderer, bullet, NULL, &bullet_pos);
        }

        SDL_RenderPresent(renderer);
    } // End while

#if defined __linux__
	close(cliSock);
	close(srvSock);
#elif defined _WIN32 || defined _WIN64
	closesocket(cliSock);							// Close the client socket
	closesocket(srvSock);							// Close the server socket
	WSACleanup();									// Terminate use of Winsock 2 DLL
#endif
	SDL_WaitThread(thread_id_server, NULL);			// JOR SDL Thread replaces pthread
	SDL_WaitThread(thread_id_client, NULL);			// Make sure thread finishes before application closes
	SDL_WaitThread(thread_id_server_send, NULL);
    SDL_DestroyTexture(tex);
    SDL_DestroyTexture(bullet);
    SDL_DestroyTexture(map);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
