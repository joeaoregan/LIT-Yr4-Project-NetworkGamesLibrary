/*
	Joe O'Regan
	K00203642

	Audio.h

	Cross-platform class for managing game music and sound effects
*/

#ifndef AUDIO_H
#define AUDIO_H

// Unix (Ubuntu)
#if defined __linux__
#include <SDL2/SDL.h>									// SDL Library
#include <SDL2/SDL_mixer.h>								// SDL Audio Library
// Windows
#elif defined _WIN32 || defined _WIN64
//#ifdef _WIN32
#include <SDL.h> 
#include "SDL_mixer.h"
#endif

#include <vector>
#include <map>										// The sound effects are contained in a map and can be accessed using a unique id
#include <cstdlib>									// For Random Numbers
#include <ctime>									// For Random Numbers
#include <iostream>

#define SDL_MAIN_HANDLED								// Handling SDL main definition ourselves

const int NUMBER_OF_SONGS = 5;								// Total number of songs in the playlist
//const int NUM_SOUND_FX = 3;								// Number of sound effects in array

class Audio {
public:
	// Audio Singleton so only one instance of Audio class exists in scene, 
	// Gives easy access to the class and its functions
	static Audio* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new Audio();
			return s_pInstance;
		}
		return s_pInstance;
	}

	bool loadMediaAudio();								// Load the music and store in listOfMusic, and sound effects and add to m_sfxs map
	void destroy();									// Clear the audio files from memory when done

	//void loadFX(std::string fileName, std::string id);// Load sound fx to map
	bool LoadFX();									// Load sound fx to map, and return true if successful
	bool LoadMusic();								// Load the music to an array, return true if successful
	void playFX(std::string id);							// Play sound fx from map identifying with id

											// Music controls
	void musicForward();								// Returns an int to identify the song name and artist
	void musicBack();								// Returns an int to identify the song name and artist
	void playPauseMusic();								// Function to start music playing, with pause/play functionality from Game class
	void stopMusic();								// Function to stop music playing

	void identifyTrack(int songName);						// Identify the song playing

	void PlayRandomSong();
	void Fire1();
	void Fire2();

private:
	Audio();									// Constructor is private for use as a Singleton. Initializes the variables
	~Audio();									// Private destructor shuts down and cleans up the mixer API

	int currentSong;								// Play a random song when the game starts

	std::vector<Mix_Music*> listOfMusic;						// List of Music tracks
	std::map<std::string, Mix_Chunk*> m_sfxs;					// Sound effects stored in a map, accessed using the unique ID for each effect
	//std::map<std::string, Mix_Music*> m_music;					// Music stored in array. Tracks skipped forwards/backwards using integer to identify the track

	static Audio* s_pInstance;							// Single instance of Audio used as singleton so only one instance exists thoughout the game
};

#endif
