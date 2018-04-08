/*
	Joe O'Regan
	K00203642

	Audio.cpp

	Cross-platform class for managing game music and sound effects
*/

#include "Audio.h"
#include <string>																	// return a string from string array of sound effect locations, names, and error description

Audio* Audio::s_pInstance;															// Singleton so only one instance of Audio exists in the game, for easy access

const int NUM_SOUND_FX = 2;															// Number of sound effects in array

/*
	2D Array of sound effects, with path to file, 
	ID, and description for error messages
*/
std::string arrSoundEffects[NUM_SOUND_FX][3] = {
	{ "Assets/Effects/scratch.wav", "laserFX", "Laser Fire Effect" },
	{ "Assets/Effects/button1.wav", "buttonFX", "Button Select Effect" }
};

/*
	2D Array storing the path, id, and error message for each music track
*/
std::string arrMusic[NUMBER_OF_SONGS][3] = {
	// Game
	{ "Assets/Music/music1.wav", "track1ID", "Music Track 1" },						// Music file 1
	{ "Assets/Music/music2.wav", "track2ID", "Music Track 2" },						// Music file 2
	{ "Assets/Music/music3.wav", "track3ID", "Music Track 3" },						// Music file 3
};

/*
	The constructor calls Mix_OpenAudio which is 
	needed before other audio functions can be used
	The audio is initialised in the Game class init() function
*/
Audio::Audio() {
	//Mix_OpenAudio(22050, AUDIO_S16, 2, (4096 / 2));								// ***DELAY*** Initialise the mixer API
	Mix_OpenAudio(44100, AUDIO_S16, 2, 1024);										// Initialise the mixer API
}

/*
	The destructor shuts down and cleans up the mixer API
*/
Audio::~Audio() {				
	Mix_CloseAudio();																// Shutdown and cleanup the mixer API
}

/* 
	Loads the effects audio files, looping through an array of FX
	that stores the path to the file, song id, and error message to display
	An error message indicates any of the effects that do not load

	The function loads the music from files in the "Music" folder
	Adds the tracks to the listOfMusic vector, picks a random song to play, 
	and plays it

	If the file fails to load it displays an identifying error message, 
	details in music array. An ID for each track is not necessary as the song 
	is selected at random	but, is stored in case a certain part of the project 
	requires a certain audio track
*/
bool Audio::loadMediaAudio() {
	bool success = true;

	success = LoadFX();																// Load SoundFX
	//success = LoadMusic();														// Load Music 
	
	//PlayRandomSong();																// Play a random song at start of scene

	return success;
}

/*
	Load the music files, store in array, return true if successful
*/
bool Audio::LoadMusic() {
	bool success = true;

	for (int i = 0; i < NUMBER_OF_SONGS; i++) {
		Mix_Music* pMusic = Mix_LoadMUS(arrMusic[i][0].c_str());					// Load music
		if (pMusic == NULL) {
			std::cout << "Failed to load music file " << 
				arrMusic[i][2] << "! ]]or: " << Mix_GetError() << std::endl;
			success = false;
		}
		listOfMusic.push_back(pMusic);
	}

	return success;
}


/*
	Used to load sound effect, and add to the m_sfxs map
	Error message displayed if chunk to be loaded is not present
*/
bool Audio::LoadFX() {
	bool success = true;

	for (int i = 0; i < NUM_SOUND_FX; i++) {

		Mix_Chunk* pChunk = Mix_LoadWAV(arrSoundEffects[i][0].c_str());
		if (pChunk == 0) {
			std::cout << "Failed to load  " << arrSoundEffects[i][2] 
				<< "! SDL_mixer Error: " << Mix_GetError() << std::endl;
			success = false;
		}
		else
			std::cout << "Loaded Effect: " << arrSoundEffects[i][2] << std::endl;

		m_sfxs[arrSoundEffects[i][1]] = pChunk;
	}
	return success;
}

void Audio::PlayRandomSong() {
	currentSong = rand() % NUMBER_OF_SONGS;											// Play a random song on start up
	std::cout << "Playing Random Song: " << arrMusic[currentSong][2] << std::endl;

	Mix_PlayMusic(listOfMusic[currentSong], -1);									// Play the currently selected song
}

/*
	Play sound FX when required, using effect ID to load from the m_sfxs map
	playFX is called from other classes using the s_pInstance singleton

	An unsuccessful attempt was made to set the audio panning based on the
	x coordinate trigger position of the effect
*/
void Audio::playFX(std::string id) {	
	/*if (!Mix_SetPanning(1, 255, 10)) {
		printf("Mix_SetPanning: %s\n", Mix_GetError());
		// no panning, is it ok?
	} */

	Mix_PlayChannel(-1, m_sfxs[id], 0);	

	//std::cout << "Effect: " << id << " Played" << std::endl;						// Test effect playing
}

void Audio::Fire1() {
	Mix_Chunk* pChunk = Mix_LoadWAV("SFX/Swoosh1.wav");
	Mix_PlayChannel(-1, pChunk, 0);
}

void Audio::Fire2() {
	Mix_Chunk* pChunk = Mix_LoadWAV("SFX/Swoosh2.wav");
	Mix_PlayChannel(-1, pChunk, 0);
}

/*
	Skip the current song that is playing forwards
	If the song is the last song in the array, play song at start of listOfMusic
	Calls identifyTrack() to display the track details, 
	outputs current song name to console
*/
void Audio::musicForward() {														// Pick next track on the list
	if (currentSong + 1 < (int) listOfMusic.size())									// If the current song number (0 number start value) is less than the number of tracks on the list
		currentSong++;																// go to next track on list
	else
		currentSong = 0;															// or else go back to start of list

	Mix_PlayMusic(listOfMusic[currentSong], -1);
	identifyTrack(currentSong);	
}

/*
	Skips the current song that is playing backwards. 
	If current song is the first song in the array, 
	it plays the song at the end of the listOfMusic array. 
	identifyTrack() displays the track details
*/
void Audio::musicBack() {															// Pick previous track on the list
	if (currentSong > 0) currentSong--;
	else currentSong = listOfMusic.size() - 1;

	Mix_PlayMusic(listOfMusic[currentSong], -1);
	identifyTrack(currentSong);
}

/*
	This function identifies the track name
*/
void Audio::identifyTrack(int songName) {
	std::cout << "Current track: " << arrMusic[songName][2] << std::endl;
}

/* 
	Starts the music playing using the randomly selected current song
	Displays a message indicating the music is playing
	It returns the currentSong as an integer to be identified

	If the music is playing and the function is called the music is paused
	If the music is paused it is then played
	Moved pause functionality from Game class
*/
void Audio::playPauseMusic() {
	if (Mix_PlayingMusic() == 0) {													// If there is no music playing
		Mix_PlayMusic(listOfMusic[currentSong], -1);								// Function to play music
		std::cout << "Music Play" << std::endl;
		identifyTrack(currentSong);
	}
	else {																			// If music is being played
		if (Mix_PausedMusic() == 1) {												// Check if the music is paused
			Mix_ResumeMusic();														// Resume music
			std::cout << "Music Play" << std::endl;
		}
		else {																		// If the music is playing
			Mix_PauseMusic();														// Pause the music
			std::cout << "Music Paused" << std::endl;
		}
	}
}

/*
	Stop the music playing & displa message indicating music is stopped
*/
void Audio::stopMusic() {
	Mix_HaltMusic();																// Function to stop music playing
	std::cout << "Music Stopped" << std::endl;										// Display message indicating the music has stopped playing
}

/*
	Clear the audio files from memory when done
*/
void Audio::destroy() {
	// Free the sound effects from memory & Free the music	
	/*	
	m_sfxs.clear;																	// Clear the sound effects map	

	//Free the music	
	Mix_FreeMusic(gMusic1);															// Free music
	gMusic1 = NULL;																	// Set to null
	*/
}
