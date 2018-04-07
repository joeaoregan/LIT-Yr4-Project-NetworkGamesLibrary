/*
	Handle sleep() functionality in microseconds
*/

#ifndef TIME_H
#define TIME_H

#if defined _WIN32 || defined _WIN64
#include <Windows.h>						// Sleep()
#endif

void sleepCrossPlatform(int amount);		// Function to handle sleep for both Windows and Linux

#endif