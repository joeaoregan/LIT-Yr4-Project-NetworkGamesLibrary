#include "menu.h"
#if defined _WIN32 || defined _WIN64
#include <Windows.h>
#endif
#include "Time.h"
//#include <string.h>		// strcpy_s
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

		sleepCrossPlatform(200);																// JOR Sleep for 200 microseconds

        SDL_RenderClear(renderer);
		displayTextRed(renderer, "Select Network Option", font, CENTRE_TEXT, 150);				// JOR Centre the text horizontally, using red font
		displayTextWhite(renderer, "[s]erver or [c]lient?", font, CENTRE_TEXT, CENTRE_TEXT);	// Centre the text horizontally and vertically on screen
        SDL_RenderPresent(renderer);
    }
}

/*
	Handle input from keyboard keypad
*/
void keypadInput(int *key) {
	switch (*key) {																				// JOR Keypad keys added
	case SDLK_KP_0: *key = SDLK_0; break;														// If keypad 0 key pressed, use 0 value
	case SDLK_KP_1: *key = SDLK_1; break;
	case SDLK_KP_2: *key = SDLK_2; break;
	case SDLK_KP_3: *key = SDLK_3; break;
	case SDLK_KP_4: *key = SDLK_4; break;
	case SDLK_KP_5: *key = SDLK_5; break;
	case SDLK_KP_6: *key = SDLK_6; break;
	case SDLK_KP_7: *key = SDLK_7; break;
	case SDLK_KP_8: *key = SDLK_8; break;
	case SDLK_KP_9: *key = SDLK_9; break;
	case SDLK_KP_PERIOD: *key = SDLK_PERIOD; break;												// JOR Keypad . was displaying strange character
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
						//strcpy_s(ip, 10,"127.0.0.1");											// Set the server address as localhost
						snprintf(ip, 10, "127.0.0.1");											// Replace strcpy_s
					}
					else
						ip[position] = 0;
					ok = true;
                }
            }
        }

		sleepCrossPlatform(200);																// Sleep for 200 microseconds

        SDL_RenderClear(renderer);
		displayTextRed(renderer, "Enter Server IP Address:", font, CENTRE_TEXT, 200);			// JOR Clearer instruction and centered
		displayTextWhite(renderer, "IPv4 Format, Press Enter For Localhost (127.0.0.1)", 
			font, CENTRE_TEXT, CENTRE_TEXT);													// JOR Clearer instruction and centered
        displayTextWhite(renderer, ip, font, CENTRE_TEXT, 260);
        SDL_RenderPresent(renderer);
    }
}

