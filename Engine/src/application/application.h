#pragma once

#include "defines.h"
#include "using.h"

#include "system/memory.h"

namespace sce
{
	class SCEAPI Application
	{
	private:
		c8* m_Str;

	public:
		Application();
		~Application();

		i32 Run();
	};
}