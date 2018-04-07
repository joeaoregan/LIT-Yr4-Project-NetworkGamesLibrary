#include "menu.h"
#if defined _WIN32 || defined _WIN64
#include <Windows.h>
#endif

void selectServerOrClient(SDL_Renderer *renderer, char *menu, TTF_Font *font){
    SDL_Event e;
    int pressed = false;

    while (!pressed) {
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_c) {
                    *menu = 'c';
                    pressed = true;
                } else if (e.key.keysym.sym == SDLK_s) {
                    *menu = 's';
                    pressed = true;
                }
            }
        }
		/*
#if defined __linux__
		usleep(200);
#elif defined _WIN32 || defined _WIN64
		Sleep(200/1000);
#endif
*/
		sleepCrossPlatform(200);

        SDL_RenderClear(renderer);
        disp_text(renderer, "[s]erver or [c]lient?", font, 240, 200);
        SDL_RenderPresent(renderer);
    }
}

void enterServerIP(SDL_Renderer *renderer, TTF_Font *font, char *ip) {
    memset(ip, ' ', 15);
    SDL_Event e;
    int position = 0;
    int ok = false;
    while (!ok) {
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
				// JOR Keypad keys added (not working
				switch (e.key.keysym.sym) {
				case SDLK_KP_0: e.key.keysym.sym = SDLK_0; break;
				case SDLK_KP_1: e.key.keysym.sym = SDLK_1; break;
				case SDLK_KP_2: e.key.keysym.sym = SDLK_2; break;
				case SDLK_KP_3: e.key.keysym.sym = SDLK_3; break;
				case SDLK_KP_4: e.key.keysym.sym = SDLK_4; break;
				case SDLK_KP_5: e.key.keysym.sym = SDLK_5; break;
				case SDLK_KP_6: e.key.keysym.sym = SDLK_6; break;
				case SDLK_KP_7: e.key.keysym.sym = SDLK_7; break;
				case SDLK_KP_8: e.key.keysym.sym = SDLK_8; break;
				case SDLK_KP_9: e.key.keysym.sym = SDLK_9; break;
				case SDLK_KP_PERIOD: e.key.keysym.sym = SDLK_PERIOD; break;
				}
				/*
				if (e.key.keysym.sym == SDLK_KP_0) e.key.keysym.sym = SDLK_0;
				else if (e.key.keysym.sym == SDLK_KP_1) e.key.keysym.sym = SDLK_1;
				else if (e.key.keysym.sym == SDLK_KP_2) e.key.keysym.sym = SDLK_2;
				else if (e.key.keysym.sym == SDLK_KP_3) e.key.keysym.sym = SDLK_3;
				else if (e.key.keysym.sym == SDLK_KP_4) e.key.keysym.sym = SDLK_4;
				else if (e.key.keysym.sym == SDLK_KP_5) e.key.keysym.sym = SDLK_5;
				else if (e.key.keysym.sym == SDLK_KP_6) e.key.keysym.sym = SDLK_6;
				else if (e.key.keysym.sym == SDLK_KP_7) e.key.keysym.sym = SDLK_7;
				else if (e.key.keysym.sym == SDLK_KP_8) e.key.keysym.sym = SDLK_8;
				else if (e.key.keysym.sym == SDLK_KP_9) e.key.keysym.sym = SDLK_9;
				else if (e.key.keysym.sym == SDLK_KP_PERIOD) e.key.keysym.sym = SDLK_PERIOD;
				*/
                if ((e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) || e.key.keysym.sym == SDLK_PERIOD) {
					//|| (e.key.keysym.sym >= SDLK_KP_0 && e.key.keysym.sym <= SDLK_KP_9) || e.key.keysym.sym == SDLK_KP_PERIOD) {	// JOR Keypad keys added (not working
					
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
                    ip[position] = 0;
                    ok = true;
                }
            }
        }
		/*
#if defined __linux__
		usleep(200);
#elif defined _WIN32 || defined _WIN64
		Sleep(200/1000);
#endif
*/
		sleepCrossPlatform(200);

        SDL_RenderClear(renderer);
		disp_text(renderer, "Enter Server IP Address:", font, 240, 200);	// JOR Clearer instruction
        disp_text(renderer, ip, font, 240, 230);
        SDL_RenderPresent(renderer);
    }
}

