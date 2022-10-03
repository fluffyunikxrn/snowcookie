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
	typedef enum MEMORYTAG
	{
		MEMORY_UNKNOWN = 0,			// Unknown memory allocation

		// Memory node
		MEMORY_MEMNODE,

		// Number(s)
		MEMORY_SINTEGER8,
		MEMORY_SINTEGER16,
		MEMORY_SINTEGER32,
		MEMORY_SINTEGER64,
		MEMORY_UINTEGER8,
		MEMORY_UINTEGER16,
		MEMORY_UINTEGER32,
		MEMORY_UINTEGER64,

		// Floating point number(s)
		MEMORY_FLOAT32,
		MEMORY_FLOAT64,

		// Character(s)
		MEMORY_CHAR8,
		MEMORY_CHAR16,

		// Collection(s)
		MEMORY_ARRAY,

		// TODO: Add memory tags here
		MEMORY_MAXTAGS				// Maximum number of tag types
	} MemoryTag;
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
	class PlatformAllocator;			// Platform allocator from platform.h	

	/* ==========================================================
	* NODESTATUS enumeration
	* Defines flags for used and unsed memory
	*/
	typedef enum NODESTATUS
	{
		Used,				// Tagged as occupied
		Free				// Tagged as free
	} NodeStatus;

	/* ==========================================================
	* MEMORYNODE struct
	* Keeps track of memory block chain
	*/
	typedef struct MEMORYNODE
	{
		NodeStatus		e_Status;			// Status if free or occupied
		size64			n_NodeSize;			// Size of the block in bytes
		size64			n_AllocationSize;	// Size of the block in bytes
		v8*				p_Data;				// Previous block in chain
		MEMORYNODE*		p_Prev;				// Next block in chain
		MEMORYNODE*		p_Next;				// Next block in chain
		MemoryTag		s_Tag;				// Memory tag
	} MemoryNode;

	/* ==========================================================
	* MEMORYSTATSFLAG struct
	* Sets where to increase or decrease statistics
	*/
	typedef enum MEMORYSTATSFLAG
	{
		Increment = 0,			// Add to statistics
		Decrement				// Subtract from statistics
	} MemoryStatsFlag;

	/* ==========================================================
	* MEMORYNODESTATS struct
	* Keeps track of memory stats
	*/
	typedef struct MEMORYNODESTATS
	{
		size64 TotalPointerMemoryUsed;									// Total pointer memory used
		size64 TotalAllocationMemoryUsed;								// Total allocation memory used
		size64 FlagPointerMemoryUsed[MemoryTag::MEMORY_MAXTAGS];		// Tag pointer memory used
		size64 FlagAllocationMemoryUsed[MemoryTag::MEMORY_MAXTAGS];		// Tag allocation memory used
	} MemoryNodeStats;
}
