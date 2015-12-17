// Windows
#include <Windows.h>

// STD
#include <stdio.h>
#include <iostream>

#include "DirectXGameCore.h"

// Include run-time memory checking in debug builds, so 
// we can be notified of memory leaks
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

/*
 * This function will create a console to print debug information.
 * Based on the following StackOverflow post:
 *      http://stackoverflow.com/questions/311955/redirecting-cout-to-a-console-in-windows
 */
void CreateDebugConsole(void)
{
    // Allocate a console for this application.
    AllocConsole();
    SetConsoleTitle(L"Asteroids Debug Console");

    // Redirect input/output to the console.
    FILE *pIn, *pErr, *pOut;
    freopen_s(&pIn, "CONIN$", "r", stdin);
    freopen_s(&pOut, "CONOUT$", "w", stdout);
    freopen_s(&pErr, "CONOUT$", "w", stderr);

    // Clear the error state of the std::iostream objects.
    std::cout.clear();
    std::cerr.clear();
    std::cin.clear();
    std::wcout.clear();
    std::wcerr.clear();
    std::wcin.clear();

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	GetConsoleScreenBufferInfo(h, &coninfo);
	coninfo.dwSize.Y = 500;                         // Maximum mumber of lines for the output console.
	SetConsoleScreenBufferSize(h, coninfo.dwSize);
}

// --------------------------------------------------------
// Win32 Entry Point - Where your program starts
// --------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
#if defined(DEBUG) | defined(_DEBUG)
	// Enable run-time memory check for debug builds.
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

    CreateDebugConsole();
#endif

	// Create the game object.
	DirectXGameCore game(hInstance);
	
	// This is where we'll create the window, initialize DirectX, 
	// set up geometry and shaders, etc.
	if( !game.Init() )
		return 0;

	// All set to run the game loop
	return game.Run();
}
