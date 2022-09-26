/*
* File name: Using.h
* File desc: Decleration of using type(s).
* Author: Chloe Tunrer (chloeturner@tuta.io)
* Copyright: Copyright 2022 Chloe Tunrer
*/

#pragma once

#include "defines.h"

/* Numbers
* Integer bit size(s) chart:
* Data model	Char	Short-Int	Int		Long-Int	Long-Long-Int	Pointer(s)
*  ILP32		8		16			32		32			64				32* 
*  LLP64		8		16			32		32			64				64
*  LP64			8		16			32		64			64				64
*  ILP64		8		16			64		64			64				64
*  SILP64		64		64			64		64			64				64* 
* https://en.wikipedia.org/wiki/64-bit_computing#64-bit_data_models
*/
#if defined(SCE_PLATFORM_LLP64)
using i8 = signed char;					// 08-bit wide
using i16 = signed short int;			// 16-bit wide
using i32 = signed int;					// 32-bit wide
using il32 = signed long int;			// 32-bit wide
using i64 = signed long long int;		// 64-bit wide

using u8 = unsigned char;				// 08-bit wide
using u16 = unsigned short int;			// 16-bit wide
using u32 = unsigned int;				// 32-bit wide
using ul32 = unsigned long int;			// 32-bit wide
using u64 = unsigned long long int;		// 64-bit wide

using size64 = unsigned long long int;	// 64-bit wide
#elif defined(SCE_PLATFORM_LP64)
using i8 = signed char;					// 08-bit wide
using i16 = signed short int;			// 16-bit wide
using i32 = signed int;					// 32-bit wide
using i64 = signed long int;			// 64-bit wide
using ill64 = signed long long int;		// 64-bit wide

using u8 = unsigned char;				// 08-bit wide
using u16 = unsigned short int;			// 16-bit wide
using u32 = unsigned int;				// 32-bit wide
using u64 = unsigned long int;			// 64-bit wide
using ull64 = unsigned long long int;	// 64-bit wide

using size64 = unsigned long long int;	// 64-bit wide
#endif // !Integer bit size(s)

// Floating-point number(s)
using f32 = float;						// 32-bit wide
using f64 = double;						// 64-bit wides

// Boolean
using b8 = bool;						// 08-bit wide
using b32 = i32;						// 32-bit wide

// Character(s)
using c8 = char;						// 08-bit wide
using c16 = wchar_t;					// 16-bit wide

// Pointer(s)
using v8 = void;

#if not defined(_NEW_)
#include <new>
#endif // !_NEW_
