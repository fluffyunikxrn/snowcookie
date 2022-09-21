#pragma once

#include "defines.h"
#include "using.h"

namespace sce
{
	class SCEAPI Application
	{
	public:
		Application();
		~Application();

		int Run();
	};
}