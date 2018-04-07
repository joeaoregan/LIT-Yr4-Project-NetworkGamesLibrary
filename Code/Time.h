#ifndef TIME_H
#define TIME_H

#if defined _WIN32 || defined _WIN64
#include <Windows.h>						// Sleep()
#endif

/*
	Function to handle sleep for both Windows and Linux
*/
void sleepCrossPlatform(int amount) {
#if defined __linux__
	usleep(amount);							// Linux value measured in microseconds
#elif defined _WIN32 || defined _WIN64
	Sleep(amount / 1000);					// Windows value measured in milliseconds
#endif
}

#endif