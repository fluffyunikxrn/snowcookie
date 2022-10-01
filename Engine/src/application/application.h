/*
* File name: Application.h
* File desc: Where the application runs.
* Author: Chloe Tunrer (chloeturner@tuta.io)
* Copyright: Copyright 2022 Chloe Tunrer
*/

#pragma once

#include "defines.h"
#include "using.h"

//#include "system/memory.h"

namespace sce
{
	class SCEAPI Application
	{
	private:

	public:
		Application();
		~Application();

		i32 Run();
	};
}