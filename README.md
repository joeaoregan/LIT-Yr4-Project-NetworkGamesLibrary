# SDL Game with CLient / Server Communication over UDP
## With communication loops on separate threads for Client and Server
### Original version by Daniel Linssen
### Adapted to work run on Windows by Joe O'Regan (tested on windows 10)

---

#### Changes Made

1. Replaced PThread functionality with SDL_Thread for cross-platform use
2. Added Winsock compatibility for communication on windows systems
3. Time functionality fixed to work with windows 
    * Sleep(), handling time in milliseconds, replaces Usleep() (microseconds)
    * SDL_GetTicks() replaces gettimeofday() for timing
4. Network.c: INADDR_ANY was causing issues, so it has been replaced with 127.0.0.1 for server
5. Added sprite change of direction passed from the Server to the Client based on key press received by Server
6. Identify local player with alternate sprite colour
7. Added more tiles and collision checks
8. Using **[JOR_Net](https://github.com/joeaoregan/JOR_Net) UDP Network Games Library** to communicate between client and server

---

#### Screen Shots

![SDL Game: With Cross-Platform UDP Networking](https://raw.githubusercontent.com/joeaoregan/sdl-game/master/Screenshots/20180406_CrossPlatformImplemented.png "SDL Game: With Cross-Platform UDP Networking")
Game communicating between windows and VirtualBox running Ubuntu Linux

![SDL Game: Local Player identifed with alternate sprite](https://raw.githubusercontent.com/joeaoregan/sdl-game/master/Screenshots/20180408_LocalPlayerIdentifiedByRedSprite.png "SDL Game: Local Player identifed with alternate sprite")
Local Player identifed with alternate sprite

![SDL Game: Additional Tiles](https://raw.githubusercontent.com/joeaoregan/sdl-game/master/Screenshots/20180408_AdditionalTiles.png "SDL Game: Additional Tiles")
Additional tiles and tile collision checks

---

#### Original README.md text:

# sdl-game
Pure C+SDL game :video_game: client-server architecture over UDP sockets

font by [Daniel Linssen](https://managore.itch.io/m5x7)
