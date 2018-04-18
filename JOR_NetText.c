/*
	Joe O'Regan
	K00203642

	JORNetText.cpp

	Format the Server text for both Linux Terminal and Windows Command Prompt
	Console text is displayed in colour
*/

#include "stdafx.h"
#include "JOR_NetText.h"
#include <stdio.h>

//#define RED   "\x1B[31m"
//#define GREEN "\x1B[32m"
//#define YELO  "\x1B[33m"
//#define BLUE  "\x1B[34m"
//#define MGNT  "\x1B[35m"
//#define CYAN  "\x1B[36m"
//#define NORM  "\x1B[0m"

/*
	Format text based depending on platform
*/
void JOR_NetTextCoords(int id, int x, int y){
// Format output for Linux terminal
#if defined __linux__
	printf("\x1B[34mClient ID:\x1B[0m\t%d\t\x1B[34mX:\x1B[0m\t%d\x1B[34m\tY:\x1B[0m\t%d\n",id, x, y);	// Display Coordinates

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
	else if (colour == 6)						// Gold 
		textColour = "\x1B[33m";
	else if (colour == 9)						// Blue
		textColour = "\x1B[34m";
	else if (colour == 10)						// Green
		textColour = "\x1B[32m";
	else if (colour == 11)						// Cyan
		textColour = "\x1B[36m";
	else if (colour == 12)						// Red
		textColour = "\x1B[31m";
	else
		textColour = "\x1B[0m";					// Normal

	// Format output for Linux terminal
	//printf("%s%s%s",textColour,msg,NORM);		// Display message in single colour
	printf("%s%s\x1B[0m",textColour,msg);		// Display message in single colour
 
#elif defined _WIN32 || defined _WIN64	
	// Format output for Windows command prompt
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Standard output device

	SetConsoleTextAttribute(hConsole, colour);	// SetConsoleTextAttribute() sets foreground (text) and background colour for characters in the console
	printf("%s", msg);							// Display message in single colour
	SetConsoleTextAttribute(hConsole, NORM);	// Reset to original colour scheme
#endif
}
