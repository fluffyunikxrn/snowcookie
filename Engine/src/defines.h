/*
* File name: Defines.h
* File desc: Preprocessor defines for catching specific system informaiton.
* Author: Chloe Tunrer (chloeturner@tuta.io)
* Copyright: Copyright 2022 Chloe Tunrer
*/

#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define SCE_PLATFORM_DEBUG 1
#elif defined(RELEASE) || defined(_NDEBUG) || defined(NDEBUG)
#define SCE_PLATFORM_RELEASE 1
#else
#error "unknown configuration"
#endif

// Character set(s)
#if defined(_UNICODE) || defined(UNICODE)
#define SCE_PLATFORM_UNICODE 1
#else
#define SCE_PLATFORM_MULTIBYTE 1
#endif // !Character set

// Architecture
#if defined(_M_AMD64) || defined(_M_X64)
#define SCE_PLATFORM_64BIT 1
#else
#error "architechture not supported"
#endif // !Architecture

// Operating System(s)
#if defined(_WIN64) || defined(_WIN32) 
#include <Windows.h>
#include <windowsx.h>
#define SCE_PLATFORM_WINDOWS 1
#define SCE_PLATFORM_OS_NAME "Windows"
#define SCE_PLATFORM_OS_FULL_NAME = "Microsoft Windows"
// Compilier(s)
#if defined(_MSC_VER) || defined(_MSC_FULL_VER)	// microsoft
#define SCE_PLATFORM_COMPILER_MSC 1
#define SCE_PLATFORM_COMPILER_NAME "Microsoft Visual C/C++ Compiler"
#define SCE_PLATFORM_LLP64 1
#elif defined(__GNUC__)	// gnu
#define SCE_PLATFORM_COMPILER_GCC 1
#define SCE_PLATFORM_COMPILER_NAME "GNU GCC C/C++ Compiler"
#if defined(__MINGW64__) || defined(__MINGW32__)
#define SCE_PLATFORM_LLP64 1
#elif defined(__CYGWIN__)
#define SCE_PLATFORM_LP64 1
#endif // !MINGW
#else
#error "compiler not recognised"
#endif // !Compilier(s)
#elif defined(__linux__)
//#include <xlib.h>
#define SCE_PLATFORM_WINDOWS 1
#define SCE_PLATFORM_OS_NAME aa"Linux"
#define SCE_PLATFORM_LP64 1
#else
#error "operating system not supported"
#endif // !Operating System(s)

// DLL-Linkage
#if defined(SCE_EXPORT)	// export
#if defined(SCE_PLATFORM_COMPILER_MSC)	// microsoft
#define SCEAPI __declspec(dllexport)
#elif defined(SCE_PLATFORM_COMPILER_GCC)	// gnu
#define SCEAPI __attribute__((visibility("default")))
#endif // !SCE_PLATFORM_COMPILER
#elif defined(SCE_IMPORT)	// import
#if defined(SCE_PLATFORM_COMPILER_MSC)	// microsoft
#define SCEAPI __declspec(dllimport)
#elif defined(SCE_PLATFORM_COMPILER_GCC)	// gnu
#define SCEAPI __attribute__((visibility("default")))
#endif // !SCE_PLATFORM_COMPILER
#endif // !DLL-Linkage
