# Year 4 Games Design & Development 
# Project: Networked Games Library
### Joe O'Regan
### K00203642
### 21/01/2018

![Feature 1: Simple Game](https://raw.githubusercontent.com/joeaoregan/Yr4-Project-Networked-Games-Library/master/Screenshots/Feature1.png "Simple 2D Side-scrolling C++ game created using SDL2")
Simple 2D side-scrolling game in C++

---

## Feature 1: Game

###### 08/12/2017

Simple 2D side-scrolling C++ game. Created in Linux (Ubuntu) using SDL2

![Feature 2: Networking](https://raw.githubusercontent.com/joeaoregan/Yr4-Project-Networked-Games-Library/master/Screenshots/Feature2.png "Coordinates and shots fired data sending using UDP to Server")
Using UDP to communicate Player position and indicate shots fired, from Game Client to Server

---

## Feature 2: Networking

###### 17/01/2018

Game Client sends data to Server via UDP. Player coordinates update for X and Y, shots fired indicated


###### Tutorials used in creating the game:

[LazyFoo SDL2 Tutorials](http://lazyfoo.net/tutorials/SDL/)

[Install instructions for SDL2](http://lazyfoo.net/tutorials/SDL/01_hello_SDL/linux/index.php)

[SDL2 Extension Libraries (sdl_image png etc., sdl_ttf fonts, sdl_mixer audio) ](http://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/linux/index.php)

---


## Description:

##### 1-Game

Functions for playing the game. Includes main game loop. Game singleton is used to access game functionality outside the class

##### 2-GameObject

Base class for creating game objects

##### 3-Laser

Spawn, move, and render Laser game objects

##### 4-Main

Game loop. Call init(), loadMedia(), loop on update(), and close() when done. 

##### 5-Player

Create, move, and render a Player game object

##### 6-Socket

UDP Socket functions. Create socket, and send data

##### 7-Texture

Handle Texture functionality

---

## Running:

```c.
./projgame
```

###### Compile command:
```c
make all
```
Ensure dependendent files are in the root directory, and Assets directories

---

## Header Files:

| No. | File        | Description |
| --- | ------------- |-------------|
| 1 | **Audio.h** | Game sound effects and music |
| 2 | **Background.h** | Scrolling background |
| 3 | **Game.h** | Functions for playing the game |
| 4 | **GameObject.h** | Base class for game objects |
| 5 | **GameState.h** | Abstract base class for game states |
| 6 | **GameStateMachine.h** | Handle changing between states |
| 7 | **Input.h** | Handle input from keyboard, mouse, and gamepad |
| 8 | **Laser.h** | Create, move, and render Laser objects |
| 9 | **MainMenuState.h** | Menu state header file |
| 10 | **MenuButton.h** | Handle button clicks |
| 11 | **MenuState.h** | Abstract base class for menu states |
| 12 | **Player.h** | Create, move, and render Player objects |
| 13 | **PlayState.h** | Play state header file|
| 14 | **Socket.h** | UDP Socket functions |
| 15 | **Texture.h** | Texture functionality |

## Class Files:

| No. |File        | Description           |
| --- | ------------- |-------------|
| 1 | **Audio.cpp** | Fuctions for handling game audio |
| 2 | **Game.cpp** | Functions for playing the game |
| 3 | **GameStateMachine.cpp** | Functions for changing state |
| 4 | **Input.cpp** | Functions for handling input |
| 5 | **Laser.cpp** | Create, move, and render Laser objects |
| 6 | **Main.cpp** | Main entry point to the game, and game loop |
| 7 | **MainMenuState.cpp** | Functions for handling the menu state |
| 8 | **MenuButton.cpp** | Functions for handling button clicks |
| 9 | **Player.cpp** | Create, move, and render Player objects |
| 10 | **PlayState.cpp** | Functions for handling the play state |
| 11 | **Server.c** | UDP Server receives data from game |
| 12 | **Texture.cpp** | Texture functionality |
 
## Setup SDL Libraries:

### SDL Library:
```c
apt-cache search libsdl2
sudo apt-get install libsdl2-dev
```

### Image Library:
```c
apt-cache search libsdl2-image
sudo apt-get install libsdl2-image-dev
```

### Audio Library:
```c
apt-cache search libsdl2-mixer
sudo apt-get install libsdl2-mixer-dev
```

### True Type Fonts (Text) Library:
```c
apt-cache search libsdl2-ttf
sudo apt-get install libsdl2-ttf-dev
```




