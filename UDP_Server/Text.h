/*
	Joe O'Regan
	K00203642

	Text.h

	Format the Server text for both Linux Terminal and Windows Command Prompt
*/

#include <stdio.h>

// Linux
#if defined __linux__
// Colours
#define RED   "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELO  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define MGNT  "\x1B[35m"
#define CYAN  "\x1B[36m"
#define NORM  "\x1B[0m"
// Windows
#elif defined _WIN32 || defined _WIN64
#include <windows.h>   			// WinApi header
// Colours
#define MGNT 5
#define BLUE 9
#define RED 12
#define NORM 15
#endif


//void printCoords(char* name, int x, int y){
void printCoords(int id, int x, int y){
#if defined __linux__
	// Format output for Linux terminal
	//printf("%sPlayer:%s\t%s\t%sX:%s\t%d%s\tY:%s\t%d\n", BLUE, NORM, name, BLUE, NORM, x, BLUE, NORM, y);	// Display Coordinates
	printf("%sPlayer:%s\t%d\t%sX:%s\t%d%s\tY:%s\t%d\n", BLUE, NORM, id, BLUE, NORM, x, BLUE, NORM, y);	// Display Coordinates

#elif defined _WIN32 || defined _WIN64
	// Format output for Windows command prompt
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, BLUE);
	printf("Player:\t");
	SetConsoleTextAttribute(hConsole, NORM);
	//printf(name);
	printf(id);
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

void printColour(char* msg, int colour){
	//printf("colour: %d\n", colour);
#if defined __linux__
	char* textColour;
	if (colour == 5)				// Magenta 
		textColour = "\x1B[35m";
	else if (colour == 9)				// Blue
		textColour = "\x1B[34m";
	else if (colour == 12)				// Red
		textColour = "\x1B[31m";
	else
		textColour = "\x1B[0m";			// Normal

	// Format output for Linux terminal
	printf("%s%s%s\n",textColour,msg,NORM);		// Display message in single colour
 
#elif defined _WIN32 || defined _WIN64	
	// Format output for Windows command prompt
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, colour);
	printf("%s\n", msg);				// Display message in single colour
	SetConsoleTextAttribute(hConsole, NORM);
#endif
}
