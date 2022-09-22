#pragma once

#include "defines.h"
#include "using.h"

#ifndef __OSTREAM__
#include <ostream>
#endif // !__OSTREAM__

namespace sce
{
	class SCEAPI Logger : std::ostream
	{
	public:
		Logger(std::streambuf* buf); 
		~Logger();
	};
}
