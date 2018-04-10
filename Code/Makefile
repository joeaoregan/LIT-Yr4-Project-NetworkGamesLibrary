# Joe O'Regan
# K00203642
# Create netgame - Platform game using JOR_Net UDP Network Games Library

build: main.c GameObject.c ClientThread.c ServerThreads.c Input.c physic.c list.c Text.c menu.c SDLFunctions.c HUD.c
	gcc -o netgame main.c GameObject.c ClientThread.c ServerThreads.c Input.c physic.c list.c Text.c menu.c SDLFunctions.c HUD.c `sdl2-config --cflags --libs` -lSDL2_ttf -I. -I../../ -Wall -L../../bin/shared -lJOR_Net 
