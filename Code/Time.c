#include "Time.h"
#if defined __linux__
#include <unistd.h>							// usleep()
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
