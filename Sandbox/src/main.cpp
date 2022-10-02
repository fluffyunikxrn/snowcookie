/*
* File name: Main.h
* File desc: Entry-point.
* Author: Chloe Tunrer (chloeturner@tuta.io)
* Copyright: Copyright 2022 Chloe Tunrer
*/

/*
* ===========================================================
* Include(s)
* ===========================================================
*/
#include "sce_engine.h"

#include <iomanip>

/* ==========================================================
* Sandbox <- Application class
* Base class for all applications within the executable
* files.
* Exported.
*/
class Sandbox : public sce::Application
{
public:
	/* Application constructor
	* Default
	*/
	Sandbox()
	{
#if defined(SCE_PLATFORM_DEBUG)
		std::wcout << __FUNCTIONW__ << std::endl;
#endif
	}

	/* Application destructor
	*/
	~Sandbox()
	{
#if defined(SCE_PLATFORM_DEBUG)
		std::wcout << __FUNCTIONW__ << std::endl;
#endif
	}
};

/* Creates the application by linking the main function back.
* Exported from Engine in entry-point.h, decleration in 
* application.h.
* around into the engine.
* @param argc: The number of arguements supplied
* @param argv: The vector containing the value of the arguements
*/
sce::Application* sce::CreateApplication(i32 argc, c8** argv)
{
#if defined(SCE_PLATFORM_DEBUG)
	std::wcout << __FUNCTIONW__ << std::endl;
#endif

	return new Sandbox();
}
