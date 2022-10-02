/*
* File name: Application.h
* File desc: Where the application runs.
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

/*
* ===========================================================
* Namespace: sce
* ===========================================================
*/
namespace sce
{
	/* ==========================================================
	* Application class
	* Base class for all applications within the executable 
	* files.
	* Exported.
	*/
	class SCEAPI Application
	{
	public:
		/* Application constructor
		* Default
		*/
		Application();

		/* Application destructor
		*/
		virtual ~Application();

		/* Runs the main application loop
		*/
		virtual i32 Run();

	private:
		/* Sets up the class for use
		*/
		virtual v8 Setup();

		/* Shuts down the class
		*/
		virtual v8 Shutdown();
	};

	/* Creates the application by linking the main function back
	* around into the engine.
	* @param argc: The number of arguements supplied
	* @param argv: The vector containing the value of the arguements
	*/
	Application* CreateApplication(i32 argc, c8** argv);
}