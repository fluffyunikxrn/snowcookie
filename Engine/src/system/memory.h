/*
* ===========================================================
* File name:	Memory.h
* File desc:	Memory hanlding
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
	/* ==========================================================
	* MEMORYFLAG enumeration
	* Defines flags for used and unsed memory
	*/
	SCEAPI typedef enum MEMORYFLAG
	{
		// Empty memory location
		MEMORY_EMPTY = 0,				// 0

		// Unknown memory allocation
		MEMORY_UNKNOWN,					// 1

		// Memory node
		MEMORY_MEMNODE,					// 2

		// Number(s)
		MEMORY_SINTEGER8,				// 3
		MEMORY_SINTEGER16,				// 4
		MEMORY_SINTEGER32,				// 5
		MEMORY_SINTEGER64,				// 6
		MEMORY_UINTEGER8,				// 7
		MEMORY_UINTEGER16,				// 8
		MEMORY_UINTEGER32,				// 9
		MEMORY_UINTEGER64,				// 10

		// Floating point number(s)
		MEMORY_FLOAT32,					// 11
		MEMORY_FLOAT64,					// 12

		// Character(s)
		MEMORY_CHAR8,					// 13
		MEMORY_CHAR16,					// 14

		// Collection(s)
		MEMORY_ARRAY,					// 15

		// TODO: Add memory tags here
		MEMORY_MAXTAGS				// Maximum number of tag types
	} MemoryFlag;

	/* ==========================================================
	* MEMORYSIZES enumeration
	* Defines sizes for standard measuments in computing.
	*/
	SCEAPI typedef enum MEMORYSIZES
	{
		MEMORY_BIT = 0,
		MEMORY_BYTE,

		MEMORY_KILOBYTE,
		MEMORY_KIBIBYTE,

		MEMORY_MEGABYTE,
		MEMORY_MEBIBYTE,

		MEMORY_GIGABYTE,
		MEMORY_GIBIBYTE,

		MEMORY_TERABYTE,
		MEMORY_TEBIBYTE,

		MEMORY_PETABYTE,
		MEMORY_PEBIBYTE,
	} MemorySizes;

	/* ==========================================================
	* MEMORYDECBIN enumeration
	* Sets whether to get decimal or binary stats for memory 
	* nodes.
	*/
	SCEAPI typedef enum MEMORYDECBIN
	{
		Binary = 0,
		Decimal
	} MemoryDecBin;

	/* ==========================================================
	* MEMORYSIZERESULT struct
	* Return type for memory stats
	*/
	SCEAPI typedef struct MEMORYSIZERESULT
	{
		c8 ch_Unit[4];
		f64 n_Size;
	} MemorySizeResult;

	/* Conerts a memory tag id to a name string
	* @param sFlag: Input tag id
	* @return Name of the tag
	*/
	const c16* sce_MemoryTagToStringW(MemoryFlag sFlag);

	/* Conerts a memory tag id to a name string
	* @param sFlag: Input tag id
	* @return Name of the tag
	*/
	const c8* sce_MemoryTagToStringA(MemoryFlag sFlag);

	/* Converts memory status to string
	* @param sStatus: Flag id
	* @return String indicating if free or used
	*/
	const c8* sce_NodeStatusToStringA(MemoryFlag sFlag);

	/* Converts memory status to string
	* @param sStatus: Flag id
	* @return String indicating if free or used
	*/
	const c16* sce_NodeStatusToStringW(MemoryFlag sFlag);

	/* Gest a standard copmuting memasurement.
	* @param eSize: Id for requested size
	* @return The memory size as 64 bit unsigned integer
	*/
	SCEAPI const size64 sce_MemorySize(MemorySizes eSize);	

	/*
	* ===========================================================
	* Namespace: sce::sys
	* ===========================================================
	*/
	namespace sys
	{
		static const c8* g_szMemoryStringArray[MemoryFlag::MEMORY_MAXTAGS]
			=
		{
			"MEMORY_EMPTY",				// 0
			"MEMORY_UNKNOWN",			// 1
			"MEMORY_MEMNODE",			// 2
			"MEMORY_SINTEGER8",			// 3
			"MEMORY_SINTEGER16",		// 4
			"MEMORY_SINTEGER32",		// 5
			"MEMORY_SINTEGER64",		// 6
			"MEMORY_UINTEGER8",			// 7
			"MEMORY_UINTEGER16",		// 8
			"MEMORY_UINTEGER32",		// 9
			"MEMORY_UINTEGER64",		// 10
			"MEMORY_FLOAT32",			// 11
			"MEMORY_FLOAT64",			// 12
			"MEMORY_CHAR8",				// 13
			"MEMORY_CHAR16",			// 14
			"MEMORY_ARRAY",				// 15
		};

		static const c8* g_szMemoryStatusArray[2]
			=
		{
			"empty",
			"occupied"			
		};
	}

	/* Gest a standard copmuting memasurement.
	* @param eSize: Id for requested size
	* @return The memory size as 64 bit unsigned integer
	*/
	SCEAPI const MemorySizeResult sce_MemorySizeTest(size64 nSize, MemoryDecBin eDecBin);
}

/*
* ===========================================================
* Namespace: sce::sys
* ===========================================================
*/
namespace sce::sys
{
	/*
	* ===========================================================
	* Forward decleration(s)
	* ===========================================================
	*/
	class PlatformAllocator;				// Platform allocator from platform.h

	/* ==========================================================
	* MEMORYNODE struct
	* Keeps track of memory block chain
	*/
	typedef struct MEMORYNODE
	{
		size64			n_NodeSize;			// Size of the block in bytes
		size64			n_AllocationSize;	// Size of the block in bytes
		v8*				p_Data;				// Previous block in chain
		MEMORYNODE*		p_Prev;				// Next block in chain
		MEMORYNODE*		p_Next;				// Next block in chain
		MemoryFlag		s_Flag;				// Memory tag
	} MemoryNode;

	/* ==========================================================
	* MEMORYNODESTATS struct
	* Keeps track of memory stats.
	* Exported.
	*/
	SCEAPI typedef struct MEMORYNODESTATS
	{
		size64 TotalPointerMemoryUsed;									// Total pointer memory used
		size64 TotalAllocationMemoryUsed;								// Total allocation memory used
		size64 FlagPointerMemoryUsed[MemoryFlag::MEMORY_MAXTAGS];		// Tag pointer memory used
		size64 FlagAllocationMemoryUsed[MemoryFlag::MEMORY_MAXTAGS];	// Tag allocation memory used
	} MemoryNodeStats;
}
