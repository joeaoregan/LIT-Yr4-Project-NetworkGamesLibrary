#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#if defined __linux__
#include <unistd.h>
#elif defined _WIN32 || defined _WIN64
#include <WinSock2.h>							// Windows sockets
#include <ws2tcpip.h>							// getaddrinfo()
#endif
#include "font.h"
#include "Definitions.h"

void selectServerOrClient(SDL_Renderer *renderer, char *menu, TTF_Font *font);
void enterServerIP(SDL_Renderer *renderer, TTF_Font *font, char *ip);

#endif
