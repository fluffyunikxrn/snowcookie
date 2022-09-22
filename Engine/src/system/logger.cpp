#include "pch.h"
#include "logger.h"

sce::Logger::Logger(std::streambuf* buf) : std::ostream(buf)
{
#if defined(SCE_PLATFORM_DEBUG)
	std::wcout << __FUNCTIONW__ << std::endl;
#endif

}

sce::Logger::~Logger()
{
#if defined(SCE_PLATFORM_DEBUG)
	std::wcout << __FUNCTIONW__ << std::endl;
#endif

}
