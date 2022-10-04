/*
* ===========================================================
* File name:	Console.h
* File desc:	Basic console information.
* Author:		Chloe Tunrer (chloeturner@tuta.io)
* Copyright:	Copyright 2022 Chloe Tunrer
* ===========================================================
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
	/* ===========================================================
	* CONSOLESTREAM enumeration
	* Used to select console stream
	*/
	typedef enum CONSOLESTREAM
	{
		CONSOLE_STDERROR = 0,
		CONSOLE_STDOUT,
		CONSOLE_STDIN,
	} ConsoleStream;

	/* ===========================================================
	* CONSOLETEXTCOLOUR enumeration
	* Used to select colour for terminal
	*/
	typedef enum CONSOLETEXTCOLOUR
	{
		Default = 0,

		White,
		Black,

		Red,
		Green,
		Blue,
		Cyan,
		Magenta,
		Yellow,

		LightRed,
		LightGreen,
		LightBlue,
		LightCyan,
		LightMagenta,
		LightYellow,
	} ConsoleTextColour;
}
