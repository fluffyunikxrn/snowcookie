/*
* File name: Main.h
* File desc: Entry-point.
* Author: Chloe Tunrer (chloeturner@tuta.io)
* Copyright: Copyright 2022 Chloe Tunrer
*/

#include "include.h"

#include <iomanip>

int main(int argc, char&& argv)
{
#if defined(SCE_PLATFORM_DEBUG)
	std::wcout << __FUNCTIONW__ << std::endl;
#endif

	sce::Application app;
	app.Run();

	return 0;
}