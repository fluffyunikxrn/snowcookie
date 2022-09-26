/*
* ===========================================================
* File name:	Memory.h
* File desc:	Memory allocation management and operator 
*				new/delete overloading.
* Author:		Chloe Tunrer (chloeturner@tuta.io)
* Copyright:	Copyright 2022 Chloe Tunrer
* ===========================================================
*/

/*
* ======================
* Include guard(s)
* ======================
*/
#pragma once

/*
* ===========================================================
* Include(s)
* ===========================================================
*/
#include "defines.h"
#include "using.h"
#include "patterns.h"

/*
* ===========================================================
* Engine Namespace: sce
* ===========================================================
*/
namespace sce
{
	/* ==========================================================
	* MEMORYTAG struct
	* Contains all the tags relating to the type of memory stored
	*/
	typedef enum MEMORYTAG
	{
		MEMORY_UNKNOWN = 0,			// 0
		MEMORY_MEMORYNODE,			// 1
		MEMORY_INTEGER8,			// 2
		MEMORY_INTEGER16,			// 3
		MEMORY_INTEGER32,			// 4
		MEMORY_INTEGER64,			// 5
		MEMORY_FLOAT32,				// 6
		MEMORY_FLOAT64,				// 7
		MEMORY_ARRAY,				// 8
		MEMORY_STRING,				// 9
		MEMORY_APPLICATION,			// 10

		MEMORY_MAXMEMORYTAGS,
	} MemoryTag;
}

/*
* ===========================================================
* Override(s)
* ===========================================================
*/
#pragma region newdelete
// Windows CRT new/delete
#ifdef SCE_PLATFORM_WINDOWS
// New overrides(s)
_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
v8* __CRTDECL operator new(size_t nSize);
_NODISCARD _Ret_maybenull_ _Success_(return != NULL) _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
v8* __CRTDECL operator new(size_t nSize, ::std::nothrow_t const&) noexcept;
_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
v8* __CRTDECL operator new[](size_t nSize);
_NODISCARD _Ret_maybenull_ _Success_(return != NULL) _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
v8* __CRTDECL operator new[](size_t nSize, ::std::nothrow_t const&) noexcept;

// Delete overrides(s)
v8 __CRTDECL operator delete(v8 * pMem) noexcept;
v8 __CRTDECL operator delete(v8 * pMem, ::std::nothrow_t const&) noexcept;
v8 __CRTDECL operator delete[](v8* pMem) noexcept;
v8 __CRTDECL operator delete[](v8* pMem, ::std::nothrow_t const&) noexcept;
v8 __CRTDECL operator delete(v8 * pMem, size_t _Size) noexcept;
v8 __CRTDECL operator delete[](v8* pMem, size_t _Size) noexcept;
#endif // SCE_PLATFORM_WINDOWS

//// Engine specific overrides
//v8* operator new(size_t nSize, sce::MemoryTag tag);
#pragma endregion newdelete

/* 
* ===========================================================
* Engine Namespace: sce
* ===========================================================
*/
namespace sce
{
	// Forward decleration(s)
	class Memory;			// Memory class

	/* ==========================================================
	* MEMORYTAG struct
	* Contains all the tags relating to the type of memory stored
	*/
	typedef enum UPDATEMEMORYSTATS
	{
		UPDATE_PLUS = 0,		// 0
		UPDATE_MINUS,			// 1
	} UpdateMemoryStats;

	/* ==========================================================
	* MEMORYNODE struct
	* Contains all the information about a node in memory
	*/
	typedef struct MEMORYNODE
	{
		MEMORYNODE* pNext;			// Next MEMORYNODE pointer in chain
		v8* pData;					// Current pointer memory		
		size64 nPointerSize;		// Size of the pointer
		size64 nAllocationSize;		// Size of the allocated memory
		MemoryTag Tag;				// Tag type
		b8 bFree;					// True (unoccupied) False (occupied)
	} MemoryNode;

	/* ==========================================================
	* MemoryNodes class
	* 
	* Manages the memory creation, manipulation and deletion
	* and keeps the node list updated
	* 
	*/
	class MemoryNodes
	{
		friend Memory;			// Make Meory class a friend of this class

	protected:
		MemoryNode* m_StartNode;					// Beginning memory node list
		static size64 m_nLength;					// Node list length
		static size64 m_nTotalPointerMemoryUsed;	// Total pointer memory used
		static size64 m_nTotalMemoryAllocated;		// Total allocated memory used
		static size64 m_nTotalPointerByTag[];		// Total pointer memory by tag type used
		static size64 m_nTotalAllocatedByTag[];		// Total memory allocated by tag type used

	protected:
		/* Creates an empty new node, on the heap
		*/
		MemoryNode* NewNode();

		/* Validates and deletes a specific node
		* * @param pNode: memory to delete
		*/
		v8 DeleteNode(MemoryNode* pNode);

		/* Shuts down all nodes, must be called after ShutdownMemory
		*/
		v8 ShutdownNodes();

	private:
		/* Updates all the statistics in the class
		* @param pMem:
		* @param nSize:
		* @param tag:
		*/
		v8 UpdateStats(UpdateMemoryStats direction, size64 nSize, MemoryTag tag);
	};

	/* ==========================================================
	* Memory class
	* 
	* Creates a static singleton to managte memory creation,
	* tracking and statistics 
	*/
	class SCEAPI Memory : public sce::ISingleton<Memory>, public MemoryNodes
	{
	public:
		/* Creates multiple nodes specified by length
		* @param nLength: memory to be deleted
		* @return: heap memory
		*/
		static MemoryNode* CreateNodePool(size64 nLength);

		/* Creates and tracks statistics about the new memory
		* object on the heap
		* @param pMem: memory to processed
		* @param nSize: size of allocation
		* @param Tag: set the memory type
		* @return: heap memory
		*/
		static v8* NewMemory(v8* pMem, size64 nSize, MemoryTag Tag);

		/* Deletes memory at specific address
		* @param pMem: memory to be deleted
		* @return: heap memory
		*/
		static v8 DeleteMemory(v8* pMem);

		/* Shuts down all active pointers stared in nodes
		*/
		static v8 ShutdownMemory();
	};
}
