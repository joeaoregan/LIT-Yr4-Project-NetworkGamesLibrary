# SDL Game with CLient / Server Communication over UDP
## With communication loops on separate threads for Client and Server
### Original version by Daniel Linssen
### Adapted to work run on Windows by Joe O'Regan (tested on windows 10)

![Cross-Platform SDL UDP Networked Game](https://raw.githubusercontent.com/joeaoregan/sdl-game/master/Screenshots/20180406_CrossPlatformImplemented.png "SDL UDP Networked Game - Cross-Platform")

Game communicating between windows and VirtualBox running Ubuntu Linux

#### Changes Made:
1. Replaced PThread functionality with SDL_Thread for cross-platform use
2. Added Winsock compatibility for communication on windows systems
3. Time functionality fixed to work with windows 
    * Sleep(), handling time in milliseconds, replaces Usleep() (microseconds)
    * SDL_GetTicks() replaces gettimeofday() for timing
4. Network.c: INADDR_ANY was causing issues, so it has been replaced with 127.0.0.1 for server

-

# sdl-game
Pure C+SDL game :video_game: client-server architecture over UDP sockets

font by [Daniel Linssen](https://managore.itch.io/m5x7)
