/*
* ===========================================================
* File name:	Platform.cpp
* File desc:	Handle's platform specific code
* 				implimentation
* Author:		Chloe Tunrer (chloeturner@tuta.io)
* Copyright:	Copyright 2022 Chloe Tunrer
* ===========================================================
*/

/*
* ===========================================================
* Include(s)
* ===========================================================
*/
#include "pch.h"
#include "platform.h"

/* SCE Engine specific new override which creates memory with
* an tag id tag to identify it.
* @param nSize: size of stack memory
* @param sTag: Identifying tag
* @return Newly aquired memory address
*/
v8* operator new(size_t nSize, sce::MemoryTag sTag)
{
	return sce::plfm::Platform::GetInstance().AllocateMemory(nSize, sTag);
}

/*
* ===========================================================
* Operating System: Windows
* ===========================================================
*/
#if defined(SCE_PLATFORM_WINDOWS)
#if defined(SCE_PLATFORM_UNICODE)
/*
* ===========================================================
* Override(s): New and Delete
* ===========================================================
*/
#pragma region newdelete
// New overrides(s)
_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
v8* __CRTDECL operator new(size_t nSize)
{
	return sce::plfm::Platform::GetInstance().AllocateMemory(nSize, sce::MemoryTag::MEMORY_UNKNOWN);
}

_NODISCARD _Ret_maybenull_ _Success_(return != NULL) _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
v8* __CRTDECL operator new(size_t nSize, ::std::nothrow_t const&) noexcept
{
	return sce::plfm::Platform::GetInstance().AllocateMemory(nSize, sce::MemoryTag::MEMORY_UNKNOWN);
}

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
v8* __CRTDECL operator new[](size_t nSize)
{
	return sce::plfm::Platform::GetInstance().AllocateMemory(nSize, sce::MemoryTag::MEMORY_UNKNOWN);
}

_NODISCARD _Ret_maybenull_ _Success_(return != NULL) _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
v8* __CRTDECL operator new[](size_t nSize, ::std::nothrow_t const&) noexcept
{
	return sce::plfm::Platform::GetInstance().AllocateMemory(nSize, sce::MemoryTag::MEMORY_UNKNOWN);
}

// Delete overrides(s)
v8 __CRTDECL operator delete(v8* pAddr) noexcept
{
	sce::plfm::Platform::GetInstance().DeallocateMemory(pAddr);
}

v8 __CRTDECL operator delete(v8* pAddr, ::std::nothrow_t const&) noexcept
{
	sce::plfm::Platform::GetInstance().DeallocateMemory(pAddr);
}

v8 __CRTDECL operator delete[](v8* pAddr) noexcept
{
	sce::plfm::Platform::GetInstance().DeallocateMemory(pAddr);
}

v8 __CRTDECL operator delete[](v8* pAddr, ::std::nothrow_t const&) noexcept
{
	sce::plfm::Platform::GetInstance().DeallocateMemory(pAddr);
}

v8 __CRTDECL operator delete(v8* pAddr, size_t _Size) noexcept
{
	sce::plfm::Platform::GetInstance().DeallocateMemory(pAddr);
}

v8 __CRTDECL operator delete[](v8* pAddr, size_t _Size) noexcept
{
	sce::plfm::Platform::GetInstance().DeallocateMemory(pAddr);
}
#pragma endregion newdelete

/* ==========================================================
* NodeManager class
* Maanges memory allocation globally for operating system.
* Not exported.
*/

/* Setups up the list for first time use
*/
v8 sce::plfm::NodeList::SetupList()
{
#ifdef SCE_PLATFORM_DEBUG
	wprintf(L"%s\n", __FUNCTIONW__);
#endif // !SCE_PLATFORM_DEBUG

	if (!m_sStartNode)
	{
		m_sStartNode = NewNode();

		m_sStartNode->e_Status = Free;
		m_sStartNode->n_AllocationSize = 0;
		m_sStartNode->n_NodeSize = sizeof(MemoryNode);
		m_sStartNode->p_Data = nullptr;
		m_sStartNode->p_Next = nullptr;
	}
}

/* Frees and nulls all requested memory
*/
v8 sce::plfm::NodeList::ShutdownList()
{
#ifdef SCE_PLATFORM_DEBUG
	wprintf(L"%s\n", __FUNCTIONW__);
#endif // !SCE_PLATFORM_DEBUG

	MemoryNode* p_head = nullptr;
	MemoryNode* p_next = nullptr;
	p_head = m_sStartNode;

	while (p_head)
	{
		p_next = p_head->p_Next;

		if (p_head->p_Data && p_head->e_Status == Used)
		{
			if (!VirtualFree(p_head->p_Data, NULL, MEM_RELEASE))
			{
				wprintf(L"%s: %s\n", __FUNCTIONW__, L"failed to release (data) memory");
				exit(-1);
			}

			p_head->p_Data = nullptr;
			p_head->e_Status = Free;
		}

		if (!VirtualFree(p_head, NULL, MEM_RELEASE))
		{
			wprintf(L"%s: %s\n", __FUNCTIONW__, L"failed to release (node) memory");
			exit(-1);
		}

		p_head = nullptr;
		p_head = p_next;
	}
}

/* Creates a new node
* @return Newly create node.
*/
sce::plfm::MemoryNode* sce::plfm::NodeList::NewNode()
{
#ifdef SCE_PLATFORM_DEBUG
	wprintf(L"%s\n", __FUNCTIONW__);
#endif // !SCE_PLATFORM_DEBUG

	MemoryNode* p_node = nullptr;
	p_node = static_cast<MemoryNode*>(VirtualAlloc(NULL, sizeof(MemoryNode), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));

	if (!p_node)
	{
		wprintf(L"%s: %s\n", __FUNCTIONW__, L"failed to create initial node");
		exit(-1);
	}

	p_node->e_Status			= Free;
	p_node->n_AllocationSize	= 0;
	p_node->n_NodeSize			= sizeof(MemoryNode);
	p_node->p_Data				= nullptr;
	p_node->p_Next				= nullptr;
	p_node->s_Tag				= MemoryTag::MEMORY_UNKNOWN;

	return p_node;
}

/* Scan node list for an available node
* @return Free node if available or new end node
*/
sce::plfm::MemoryNode* sce::plfm::NodeList::ScanAvailable()
{
	MemoryNode* p_head = nullptr;
	p_head = m_sStartNode;

	while (p_head)
	{
		if (!p_head->p_Data && p_head->e_Status == Free)
		{
			return p_head;
		}

		if (!p_head->p_Next)
		{
			p_head->p_Next = NewNode();
			return p_head->p_Next;
		}

		p_head = p_head->p_Next;
	}
}

/* Applies the new data to node
* @param pNode: Node to apply data to
* @param pData: New data to be added
* @param nSize: Size of the stack memory
* @return True if successful
*/
b8 sce::plfm::NodeList::AllocateData(MemoryNode* pNode, v8* pData, size64 nSize, sce::MemoryTag sTag)
{
	if (!pNode->p_Data && pNode->e_Status == Free)
	{
		pNode->p_Data = pData;

		if (pNode->p_Data)
		{
			pNode->n_AllocationSize		= nSize;
			pNode->s_Tag				= sTag;
			pNode->e_Status				= Used;			

			return true;
		}
	}

	return false;
}

/* Free the node data and set flag to free
* @param pNode: Node to apply data to
* @param pData: New data to be added
* @param nSize: Size of the stack memory
* @return True if successful
*/
b8 sce::plfm::NodeList::DeallocateData(v8* pAddr)
{
	MemoryNode* p_head = nullptr;
	p_head = m_sStartNode;

	while (p_head->p_Next)
	{
		if (p_head->p_Data == pAddr)
		{
			break;
		}

		p_head = p_head->p_Next;
	}

	if (p_head->e_Status == Used)
	{
		if (!VirtualFree(p_head->p_Data, NULL, MEM_RELEASE))
		{
			wprintf(L"%s: %s\n", __FUNCTIONW__, L"failed to release (data) memory");
			exit(-1);

			return false;
		}

		p_head->p_Data		= nullptr;
		p_head->e_Status	= Free;
	}

	return true;
}

/* ==========================================================
* PlatformAllocator class
* Manages memory allocation globally for operating system.
* Not exported.
*/

/*
* ===========================================================
* Static initialisation(s)
* ===========================================================
*/
sce::plfm::MemoryNode* sce::plfm::PlatformAllocator::m_sStartNode	= nullptr;
const c16* sce::plfm::PlatformAllocator::m_szMutexName				= L"sce_plfm_alloc_mtx";
HANDLE sce::plfm::PlatformAllocator::m_hMutex						= nullptr;
b8 sce::plfm::PlatformAllocator::m_bAllocInit						= false;

/* Allocates memory to the heap
* @param nSize: Size of stack allocation
* @param tag: Memory tag identifier
* @return Address if allocated memory
*/
v8* sce::plfm::PlatformAllocator::AllocateMemory(SIZE_T nSize, sce::MemoryTag sTag)
{
#ifdef SCE_PLATFORM_DEBUG
	wprintf(L"%s\n", __FUNCTIONW__);
#endif // !SCE_PLATFORM_DEBUG

	DWORD dw_mutexCode = 0;

	if (m_hMutex)
	{
		// Wait for mutex here
		dw_mutexCode = WaitForSingleObject(m_hMutex, INFINITE);
	}
	
	Setup();

	MemoryNode* p_node = nullptr;
	p_node = ScanAvailable();

	if (!p_node)
	{
		wprintf(L"%s: %s\n", __FUNCTIONW__, L"p_node is null");
		exit(-1);
	}

	switch(dw_mutexCode)
	{ 
	case WAIT_ABANDONED:
	{
		wprintf(L"%s: %s\n", __FUNCTIONW__, L"Mutex abandoned");
		exit(-1);
	}break;
	case WAIT_OBJECT_0:	{
		v8* p_data = VirtualAlloc(NULL, nSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		AllocateData(p_node, p_data, nSize, sTag);

		if (p_node->p_Data)
		{
			if (m_hMutex)
			{
				ReleaseMutex(m_hMutex);
			}

			return p_node->p_Data;
		}
	}break;
	case WAIT_TIMEOUT:
	{
		wprintf(L"%s: %s\n", __FUNCTIONW__, L"Mutex timed out");
		exit(-1);
	}break;
	case WAIT_FAILED:
	{
		wprintf(L"%s: %s\n", __FUNCTIONW__, L"Mutex failed");
		exit(-1);
	}break;
	}

	if (m_hMutex)
	{
		ReleaseMutex(m_hMutex);
	}

	return p_node->p_Data;
}

/* Free a specific pointer address
* @param pAddr: Memory address to free
*/
v8 sce::plfm::PlatformAllocator::DeallocateMemory(v8* pAddr)
{
#ifdef SCE_PLATFORM_DEBUG
	wprintf(L"%s\n", __FUNCTIONW__);
#endif // !SCE_PLATFORM_DEBUG

	DWORD dw_mutexCode = 0;

	if (m_hMutex)
	{
		// Wait for mutex here
		dw_mutexCode = WaitForSingleObject(m_hMutex, INFINITE);
	}

	switch (dw_mutexCode)
	{
	case WAIT_ABANDONED:
	{
		wprintf(L"%s: %s\n", __FUNCTIONW__, L"Mutex abandoned");
		exit(-1);
	}break;
	case WAIT_OBJECT_0: {
		if (!pAddr)
		{
			wprintf(L"%s: %s\n", __FUNCTIONW__, L"pAddr is null");
			exit(-1);
		}

		DeallocateData(pAddr);
	}break;
	case WAIT_TIMEOUT:
	{
		wprintf(L"%s: %s\n", __FUNCTIONW__, L"Mutex timed out");
		exit(-1);
	}break;
	case WAIT_FAILED:
	{
		wprintf(L"%s: %s\n", __FUNCTIONW__, L"Mutex failed");
		exit(-1);
	}break;
	}

	if (m_hMutex)
	{
		ReleaseMutex(m_hMutex);
	}
}

/* Deallocatees all memory in the node list
*/
v8 sce::plfm::PlatformAllocator::Shutdown()
{
	// Wait for mutex here
	WaitForSingleObject(m_hMutex, INFINITE);

#ifdef SCE_PLATFORM_DEBUG
	wprintf(L"%s\n", __FUNCTIONW__);
#endif // !SCE_PLATFORM_DEBUG

	ShutdownList();

	if (m_hMutex)
	{
		ReleaseMutex(m_hMutex);
		CloseHandle(m_hMutex);
	}
	
	m_bAllocInit = false;
}

/* Sets up the allocator class */
v8 sce::plfm::PlatformAllocator::Setup()
{
#ifdef SCE_PLATFORM_DEBUG
	wprintf(L"%s\n", __FUNCTIONW__);
#endif // !SCE_PLATFORM_DEBUG

	if (!m_bAllocInit)
	{
		m_hMutex = CreateMutex(NULL, FALSE, m_szMutexName);
		SetupList();

		m_bAllocInit = true;
	}
}

#endif	// SCE_PLATFORM_UNICODE
#endif	// SCE_PLATFORM_WINDOWS

