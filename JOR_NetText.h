/*
	Joe O'Regan
	K00203642

	JOR_NetText.h

	Format the Server text for both Linux Terminal and Windows Command Prompt
*/

#pragma once

// Linux
#if defined __linux__
// Colours
//#define RED   "\x1B[31m"
//#define GREEN "\x1B[32m"
//#define YELO  "\x1B[33m"
//#define BLUE  "\x1B[34m"
//#define MGNT  "\x1B[35m"
//#define CYAN  "\x1B[36m"
//#define NORM  "\x1B[0m"
// Windows
#elif defined _WIN32 || defined _WIN64

#ifdef JORNET_EXPORTS											// Added by default to the defined preprocessor macros for the DLL projet
#define JORNET_API __declspec(dllexport)						// Modifier set on the function declarations. Tells the compiler and linker to export a function or variable from the DLL so that it can be used by other applications
#else
#define JORNET_API __declspec(dllimport)						// This modifier optimizes the import of the function or variable in an application
#endif

#include <windows.h>   											// WinApi header
#endif


#include "Definitions.h"

// Colours
#define MGNT 5
#define TEST_COL 6
#define BLUE 9
#define RED 12
#define NORM 15

// extern "C" guards only required when programming with C++
#ifdef __cplusplus
extern "C" {
#endif

	JN_MODIFIER void JOR_NetTextCoords(int id, int x, int y);	// Format text based depending on platform

	JN_MODIFIER void JOR_NetTextColour(char* msg, int colour);	// Display output message with colour

#ifdef __cplusplus
}
#endif
