#ifndef NETWORK_H
#define NETWORK_H

#if defined __linux__
#include <netinet/in.h>
#include <arpa/inet.h>
#elif defined _WIN32 || defined _WIN64
#include <WinSock2.h>							// Windows sockets
#include <ws2tcpip.h>							// getaddrinfo()
#endif
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "objects.h"

#include "JOR_Net.h"

int16_t key_state_from_player(struct Player *player);
void player_from_key_state(struct Player *player, int16_t key_state);

#endif
