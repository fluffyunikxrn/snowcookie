/*
* File name: entry-point.h
* File desc: Entry point for our engine.
* Author: Chloe Tunrer (chloeturner@tuta.io)
* Copyright: Copyright 2022 Chloe Tunrer
*/

/*
* ===========================================================
* Include guard(s)
* ===========================================================
*/
#pragma once

/*
* ===========================================================
* Include(s)
* ===========================================================
*/
#include "defines.h"
#include "using.h"

#include "application/application.h"

#include <iostream>

/*
* ===========================================================
* Platform: Windows
* ===========================================================
*/
#if defined(SCE_PLATFORM_WINDOWS)
#if defined(SCE_PLATFORM_UNICODE)

/* Creates the application by linking the main function back
* around into the engine. Exported for use in executable 
* project.
* @param argc: The number of arguements supplied
* @param argv: The vector containing the value of the arguements
*/
extern sce::Application* CreateApplication(i32 argc, c8** argv);

/* Main entry point for the entire engine.
*/
int main(int argc, char&& argv)
{
#if defined(SCE_PLATFORM_DEBUG)
	std::wcout << __FUNCTIONW__ << std::endl;
#endif

	// Create application object
	sce::Application app;

	// Run application
	app.Run();
}

#endif	// SCE_PLATFORM_UNICODE
#endif	// SCE_PLATFORM_WINDOWS
