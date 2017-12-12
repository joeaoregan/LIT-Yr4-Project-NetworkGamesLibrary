# Year 4 Games Design & Development 
# Project: Networked Games Library
### Joe O'Regan
### K00203642

![alt text](https://raw.githubusercontent.com/joeaoregan/Yr4-Project-Networked-Games-Library/master/Screenshots/Feature1.png "Simple 2D Side-scrolling C++ game created using SDL2")
Select Server Handling Two Clients

---

## Feature 1:

###### 08/12/2017

Simple 2D side-scrolling C++ game. Created in Linux (Ubuntu) using SDL2

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

Call update(), and close() when done. Game loop will go here in future iteration

##### 5-Player

Create, move, and render a Player game object

##### 6-Texture

Handle Texture functionality

---

## Running:

```c.
./gameProject
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
| 1 | **Game.h** | Functions for playing the game |
| 2 | **Laser.h** | Create, move, and render Laser objects |
| 3 | **Player.h** | Create, move, and render Player objects |
| 4 | **Texture.h** | Texture functionality |

## Class Files:

| File        | Description           |
| ------------- |-------------|
| 1. **Game.cpp** | Functions for playing the game |
| 2 | **Laser.cpp** | Create, move, and render Laser objects |
| 3 | **Main.cpp** | Main entry point to the game |
| 4 | **Player.cpp** | Create, move, and render Player objects |
| 4 | **Texture.cpp** | Texture functionality |



