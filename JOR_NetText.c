/*
	Joe O'Regan
	K00203642

	JORNetText.cpp

	Format the Server text for both Linux Terminal and Windows Command Prompt
*/

#include "stdafx.h"
#include "JOR_NetText.h"
#include <stdio.h>

/*
	Format text based depending on platform
*/
void JOR_NetTextCoords(int id, int x, int y){
// Format output for Linux terminal
#if defined __linux__
	printf("%sPlayer:%s\t%s\t%sX:%s\t%d%s\tY:%s\t%d\n", BLUE, NORM, name, BLUE, NORM, x, BLUE, NORM, y);	// Display Coordinates

// Format output for Windows command prompt
#elif defined _WIN32 || defined _WIN64
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, BLUE);
	printf("Client ID:\t");
	SetConsoleTextAttribute(hConsole, NORM);
	printf("%d", id);
	SetConsoleTextAttribute(hConsole, BLUE);
	printf("\tX:\t");
	SetConsoleTextAttribute(hConsole, NORM);
	printf("%d", x);
	SetConsoleTextAttribute(hConsole, BLUE);
	printf("\tY:\t");
	SetConsoleTextAttribute(hConsole, NORM);
	printf("%d\n", y);	
#endif
}

/*
	Display output message with colour
*/
void JOR_NetTextColour(char* msg, int colour){
	//printf("colour: %d\n", colour);
#if defined __linux__
	char* textColour;
	if (colour == 5)							// Magenta 
		textColour = "\x1B[35m";
	else if (colour == 9)						// Blue
		textColour = "\x1B[34m";
	else if (colour == 12)						// Red
		textColour = "\x1B[31m";
	else
		textColour = "\x1B[0m";					// Normal

	// Format output for Linux terminal
	printf("%s%s%s",textColour,msg,NORM);		// Display message in single colour
 
#elif defined _WIN32 || defined _WIN64	
	// Format output for Windows command prompt
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Standard output device

	SetConsoleTextAttribute(hConsole, colour);	// SetConsoleTextAttribute() sets foreground (text) and background colour for characters in the console
	printf("%s", msg);							// Display message in single colour
	SetConsoleTextAttribute(hConsole, NORM);	// Reset to original colour scheme
#endif
}
