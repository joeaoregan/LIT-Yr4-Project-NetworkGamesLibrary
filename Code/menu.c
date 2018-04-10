/*
	Modified by:	Joe O'Regan
					K00203642

	menu.c

	Added JOR_Net library for communication
	Added additional text, and keyboard keypad input
*/

#include "menu.h"
#include "Input.h"
#include "JOR_Net.h"
#include <stdio.h>	// snprintf()

void selectServerOrClient(SDL_Renderer *renderer, char *menu, TTF_Font *font) {
    SDL_Event e;
    int pressed = false;

    while (!pressed) {
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_c) {
                    *menu = 'c';																// Set game instance to client
                    pressed = true;
                } else if (e.key.keysym.sym == SDLK_s) {
                    *menu = 's';																// Set game instance to server
                    pressed = true;
                }
            }
        }

		JOR_NetSleep(200);																		// JOR Sleep for 200 microseconds

        SDL_RenderClear(renderer);
		displayTextRed(renderer, "Select Network Option", font, CENTRE_TEXT, 150);				// JOR Centre the text horizontally, using red font
		displayTextWhite(renderer, "[s]erver or [c]lient?", font, CENTRE_TEXT, CENTRE_TEXT);	// Centre the text horizontally and vertically on screen
        SDL_RenderPresent(renderer);
    }
}

void enterServerIP(SDL_Renderer *renderer, TTF_Font *font, char *ip) {
    memset(ip, ' ', 15);
	ip[15] = '\0';																				// Terminate string
    SDL_Event e;
    int position = 0, ok = false;
    while (!ok) {
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {	
				keypadInput(&(e.key.keysym.sym));												// JOR added keypad input, to make testing address input quicker		

                if ((e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) || 
					e.key.keysym.sym == SDLK_PERIOD) {					
					if (position > 14) {
                        position = 14;
                    }

                    ip[position] = e.key.keysym.sym;
                    position++;
                }

                if (e.key.keysym.sym == SDLK_BACKSPACE) {
                    position--;
                    if (position < 0) {
                        position = 0;
                    }
                    ip[position] = ' ';
                }

                if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) {		
					if (position == 0) {														// If enter is pressed without entering an address
						snprintf(ip, 10, JN_SERV_ADDR);											// Set the server address as localhost. Replace strcpy_s
					}
					else
						ip[position] = 0;
					ok = true;
                }
            }
        }

		JOR_NetSleep(200);																		// Sleep for 200 microseconds

        SDL_RenderClear(renderer);
		displayTextRed(renderer, "Enter Server IP Address:", font, CENTRE_TEXT, 200);			// JOR Clearer instruction and centered
		displayTextWhite(renderer, "IPv4 Format, Press Enter For Localhost (127.0.0.1)", 
			font, CENTRE_TEXT, CENTRE_TEXT);													// JOR Clearer instruction and centered
        displayTextWhite(renderer, ip, font, CENTRE_TEXT, 260);
        SDL_RenderPresent(renderer);
    }
}

