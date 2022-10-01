/*
* ===========================================================
* File name:	Platform.h
* File desc:	Handle's platform specific code implimentation.
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
#include "patterns.h"

/*
* ===========================================================
* Operating System: None specific
* ===========================================================
*/

/*
* ===========================================================
* Namespace: sce
* ===========================================================
*/
namespace sce
{
	/*
	* ===========================================================
	* Operating System: None specific
	* ===========================================================
	*/

	/* ==========================================================
	* MEMORYTAG enumeration
	* Defines flags for used and unsed memory
	*/
	typedef enum MEMORYTAG
	{
		MEMORY_UNKNOWN = 0,			// Unknown memory allocation

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

namespace sce::plfm
{
	/*
	* ===========================================================
	* Forward deceleration(s)
	* ===========================================================
	*/
	class NodeList;
	class PlatformAllocator;
	class Platform;

	/* ==========================================================
	* NODESTATUS enumeration
	* Defines flags for used and unsed memory
	*/
	typedef enum NODESTATUS
	{
		Used,
		Free
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
		MEMORYNODE*		p_Next;				// Next block in chain
		MemoryTag		s_Tag;				// Memory tag
	} MemoryNode;
}

/*
* ===========================================================
* Override(s): New override
* ===========================================================
*/

/* SCE Engine specific new override which creates memory with 
* an tag id tag to identify it.
* @param nSize: size of stack memory
* @param sTag: Identifying tag
* @return Newly aquired memory address
*/
v8* operator new(size_t nSize, sce::MemoryTag sTag);

	/*
	* ===========================================================
	* Operating System: Windows
	* ===========================================================
	*/
#if defined(SCE_PLATFORM_WINDOWS)
#if defined(SCE_PLATFORM_UNICODE)

	/*
	* ===========================================================
	* Windows specific include(s)
	* ===========================================================
	*/
#ifndef _NEW_
#include <new>
#endif // !_NEW_

	/*
	* ===========================================================
	* Override(s): New and Delete
	* ===========================================================
	*/
#pragma region newdelete
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
	v8 __CRTDECL operator delete(v8* pAddr) noexcept;
	v8 __CRTDECL operator delete(v8* pAddr, ::std::nothrow_t const&) noexcept;
	v8 __CRTDECL operator delete[](v8* pAddr) noexcept;
	v8 __CRTDECL operator delete[](v8* pAddr, ::std::nothrow_t const&) noexcept;
	v8 __CRTDECL operator delete(v8* pAddr, size_t _Size) noexcept;
	v8 __CRTDECL operator delete[](v8* pAddr, size_t _Size) noexcept;
#pragma endregion newdelete

	/*
	* ===========================================================
	* Namespace: sce::plfm
	* ===========================================================
	*/
namespace sce::plfm
{
	/* ==========================================================
	* NodeManager class
	* Maanges memory allocation globally for operating system.
	* Not exported.
	*/
	class NodeList
	{
	private:
		static MemoryNode* m_sStartNode;

	protected:
		/* Setups up the list for first time use
		*/
		static v8 SetupList();

		/* Frees and nulls all requested memory
		*/
		static v8 ShutdownList();

		/* Creates a new node
		* @return Newly create node.
		*/
		static MemoryNode* NewNode();

		/* Scan node list for an available node
		* @return Free node if available or new end node
		*/
		static MemoryNode* ScanAvailable();

		/* Applies the new data to node
		* @param pNode: Node to apply data to
		* @param pData: New data to be added
		* @param nSize: Size of the stack memory
		* @param sTag: Identifying tag
		* @return True if successful
		*/
		static b8 AllocateData(MemoryNode* pNode, v8* pData, size64 nSize, sce::MemoryTag sTag);

		/* Free the node data and set flag to free
		* @param pAddr: Node to apply data to
		* @return True if successful
		*/
		static b8 DeallocateData(v8* pAddr);
	};

	/* ==========================================================
	* PlatformAllocator class
	* Manages memory allocation globally for operating system. 
	* Not exported.
	*/
	class PlatformAllocator : public NodeList
	{
	private:
		static const c16* m_szMutexName;
		static HANDLE m_hMutex;
		static b8 m_bAllocInit;

	public:
		/* Allocates memory to the heap
		* @param nSize: Size of stack allocation
		* @param sTag: Memory tag identifier
		* @return Address if allocated memory
		*/
		static v8* AllocateMemory(SIZE_T nSize, MemoryTag sTag);

		/* Free a specific pointer address
		* @param pAddr: Memory address to free
		*/
		static v8 DeallocateMemory(v8* pAddr);

		/* Deallocatees all memory in the node list
		*/
		static v8 Shutdown();

	protected:
		/* Sets up the allocator class 
		*/
		static v8 Setup();
	};

	/* ==========================================================
	* Platform class
	* Manage the operatings system.
	* Exported.
	*/
	class SCEAPI Platform : public ITSSingleton<Platform>, public PlatformAllocator// : public PlatformAllocator, public ITSSingleton<Platform>
	{
		friend PlatformAllocator;//

	public:
	};
}

#endif	// SCE_PLATFORM_UNICODE
#endif	// SCE_PLATFORM_WINDOWS