#pragma once

// Unicode
#if defined(_UNICODE) || defined(UNICODE)
// Unicode character set
	#define SCE_UNICODE (1 << 0)
#else
// Multibyte character set
	#define SCE_MULTIBYTE (1 << 1)
#endif	//!Unicode

// Compiler
#if defined(_MSC_VER)
// Visual studio
	#define SCE_MSVC (1 << 0)
#endif	//!Compiler

// Operating system
#if defined(_WIN64) || defined(_WIN32)
// Windows
	#define SCE_WINDOWS (1 << 0)
#endif	//!Operating system

#if defined(SCE_WINDOWS) && defined(_MSC_VER) && defined(__SCE_EXPORT__)
// Building sce library as Win64/32 dll using microsoft visual 'C/C++' compiler
	#define SCE_API __declspec(dllexport)
#elif defined(SCE_WINDOWS) && defined(_MSC_VER) && defined(__SCE_IMPORT__)
// Calling sce library as Win64/32 dll using microsoft visual 'C/C++' compiler
	#define SCE_API __declspec(dllimport)
#else
// Building or calling sce library as a static library
	#define SCE_API
#endif	//!Operating system

// Configuration
#if defined(_DEBUG) || defined(DEBUG)
// Debug
	#define SCE_DEBUG (1 << 0)
#endif	//!Configuration
