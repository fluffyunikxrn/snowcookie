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
#include "memory.h"

/*
* ===========================================================
* Override(s): New override
* ===========================================================
* Defined in platform.cpp
/*

/* SCE Engine specific new override which creates memory with
* an tag id tag to identify it.
* @param nSize: size of stack memory
* @param sFlag: Identifying tag
* @return Newly aquired memory address
*/
v8* operator new(size_t nSize, sce::MemoryFlag sFlag);

/*
* ===========================================================
* Platform: Windows
* ===========================================================
*/
#if defined(SCE_PLATFORM_WINDOWS)

/*
* ===========================================================
* Platform: Unicode
* ===========================================================
*/
#if defined(SCE_PLATFORM_UNICODE)

/*
* ===========================================================
* Namespace: sce::sys
* ===========================================================
*/
namespace sce::sys
{ 
	/* ==========================================================
	* PlatformAllocator class
	* Manages memory allocation globally for operating system.
	* Thread safe.
	* Not exported.
	*/
	class PlatformAllocator
	{
	private:
		static LPCWSTR m_szMutexName;
		static HANDLE m_hMutex;		

		static MemoryNode* m_sStartNode;
		static size64 m_uLength;
		static MemoryNodeStats m_sStats;

	private:
		/* Creates a new node and adds to node list
		*/
		static MemoryNode* CreateNode(MemoryNode* pPrev = nullptr);

		/* Deletes a specific node, joins the list up and 
		* can be used to force the delete to take place, if
		* the node isn't free. This will also release the 
		* data stored.
		* @param pNode: Node to be processed
		* @param bForce: Indicate whether to force delete
		* @return True if succeeded
		*/
		static v8 DeleteNode(MemoryNode* pNode, b8 bForce);

		/* Commit memory and statistics to free or new node
		* @param pNode: Node to be processed
		*/
		static MemoryNode* CommitNode(MemoryNode* pNode, size64 nSize, MemoryFlag sFlag);

		/* Release memory from previously commited node
		* @param pNode: Node to be processed
		*/
		static MemoryNode* ReleaseNode(MemoryNode* pNode, size64 nSize = 0);

		/* Scan for a free node or create new one and link it
		* @return Available or free node
		*/
		static MemoryNode* ScanAvailable();

		/* Finds a specific node by address
		* @param pAddr: Address of pointer/node to find
		* @return Corresponding node, if no node found null
		*/
		static MemoryNode* FindNode(v8* pAddr);

		/* Retrieves a specific nodes starts to a string
		* @param pNode: Node to find statistics for
		* @return String containing the node statistics
		*/
		static const c16* NodeToString(MemoryNode* pNode);

	protected:
		/* Shut down all memory allocated throughout runtime
		*/
		static v8 ShutdownAllocator();

	public:
		/* Allocates new memory on heap and returns it
		* @param nSize: Size of allocation on stack
		* @param sFlag: Identifying name
		* @return Address of newly created memory
		*/
		static v8* Allocate(size64 nSize, MemoryFlag sFlag);

		/* Deallocates previous allocated memory at specified
		* address
		* @param pAddr: Address to delete
		*/
		static v8 Deallocate(v8* pAddr);

		/* Gets the size of the allocation on stack for input
		* address
		* @param pAddr: Address of tyhe memory
		* @return Size of the alocation
		*/
		static size64 AddressStackSize(v8* pAddr);

		/* Get the memory statistics
		* @return Memory statistics
		*/
		static MemoryNodeStats GetStats();

		/* Returns a string containing the stats for total 
		* memory used
		*/
		static const c16* StatsToString();
	};
}

/*
* ===========================================================
* Namespace: sce
* ===========================================================
*/
namespace sce
{
	/* ==========================================================
	* Platform class
	* Manage the operating system.
	* Exported.
	*/
	class SCEAPI Platform : public ITSSingleton<Platform>, public sce::sys::PlatformAllocator
	{
	private:
	public:
		static v8 Shutdown();
	};
}

#endif	// SCE_PLATFORM_UNICODE
#endif	// SCE_PLATFORM_WINDOWS