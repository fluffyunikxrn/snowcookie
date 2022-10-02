/*
* ===========================================================
* File name:	Platform.cpp
* File desc:	Handle's platform specific code implimentation
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
* MemoryNodeList class
* Maanges memory allocation globally for operating system.
* Not exported.
*/

/*
* ===========================================================
* Static initialisation(s)
* ===========================================================
*/
sce::plfm::MemoryNode* sce::plfm::MemoryNodeList::m_sStartNode		= nullptr;			// Beginning of the node list
size64 sce::plfm::MemoryNodeList::m_uLength							= 0;				// Length of the node list

/* Setups up the list for first time use
*/
v8 sce::plfm::MemoryNodeList::SetupList()
{
#ifdef SCE_PLATFORM_DEBUG
	printf("%s\n", __FUNCTION__);
#endif // !SCE_PLATFORM_DEBUG

	// Check if beginning node is null
	if (!m_sStartNode)
	{
		// Create new node
		m_sStartNode = NewNode();
	}
}

/* Frees and nulls all requested memory
*/
v8 sce::plfm::MemoryNodeList::ShutdownList()
{
#ifdef SCE_PLATFORM_DEBUG
	printf("%s\n", __FUNCTION__);
#endif // !SCE_PLATFORM_DEBUG

	MemoryNode* p_head = nullptr;			// Head node
	MemoryNode* p_next = nullptr;			// Next node
	
	// Set head node to beginning node
	p_head = m_sStartNode;

	// Cycle through the nodes
	while (p_head)
	{
		// Set the next node
		p_next = p_head->p_Next;

		// Check if node data and status are free
		if (p_head->p_Data && p_head->e_Status == Used)
		{
			// Free data memory
			if (!VirtualFree(p_head->p_Data, NULL, MEM_RELEASE))
			{
				// Handle errors
				printf("%s: %s\n", __FUNCTION__, "failed to release (data) memory");
				exit(-1);
			}

			p_head->p_Data = nullptr;			// Set node data to null
			p_head->e_Status = Free;			// Set status flag to Free
		}

		// Free node memory
		if (!VirtualFree(p_head, NULL, MEM_RELEASE))
		{
			// Handle errors
			printf("%s: %s\n", __FUNCTION__, "failed to release (node) memory");
			exit(-1);
		}

		p_head = nullptr;		// Set node memory to null
		p_head = p_next;		// Set head to next node address
	}
}

/* Creates a new node
* @return Newly create node.
*/
sce::plfm::MemoryNode* sce::plfm::MemoryNodeList::NewNode()
{
#ifdef SCE_PLATFORM_DEBUG
	printf("%s\n", __FUNCTION__);
#endif // !SCE_PLATFORM_DEBUG

	MemoryNode* p_node = nullptr;		// New node
	
	// Create new node and store address
	p_node = static_cast<MemoryNode*>(VirtualAlloc(NULL, sizeof(MemoryNode), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));

	// Check if node was created successfully
	if (!p_node)
	{
		// Handle errors
		printf("%s: %s\n", __FUNCTION__, "failed to create initial node");
		exit(-1);
	}

	p_node->e_Status			= Free;							// Set node status
	p_node->n_AllocationSize	= 0;							// Set node allocation size
	p_node->n_NodeSize			= sizeof(MemoryNode);			// Set node size
	p_node->p_Data				= nullptr;						// Set node data
	p_node->p_Next				= nullptr;						// Set node next node
	p_node->s_Tag				= MemoryTag::MEMORY_UNKNOWN;	// Set node identifying tag

	// Update length
	m_uLength++;

	return p_node;
}

/* Scan node list for an available node
* @return Free node if available or new end node
*/
sce::plfm::MemoryNode* sce::plfm::MemoryNodeList::ScanAvailable()
{
	MemoryNode* p_head = nullptr;			// Head node

	// Set head node to beginning node
	p_head = m_sStartNode;

	// Cycle through the nodes
	while (p_head)
	{
		// Check if node data and status are free
		if (!p_head->p_Data && p_head->e_Status == Free)
		{
			break;
		}

		// Check if node needs extending
		if (!p_head->p_Next)
		{
			p_head->p_Next = NewNode();			// Create new node
			p_head = p_head->p_Next;			// Set head to next node
			break;
		}

		p_head = p_head->p_Next;				// Set head to next node	
	}

	return p_head;
}

/* Applies the new data to node
* @param pNode: Node to apply data to
* @param pData: New data to be added
* @param nSize: Size of the stack memory
* @return True if successful
*/
b8 sce::plfm::MemoryNodeList::AllocateData(MemoryNode* pNode, v8* pData, size64 nSize, sce::MemoryTag sTag)
{
	// Check if node data and status are free
	if (!pNode->p_Data && pNode->e_Status == Free)
	{
		// Set node data
		pNode->p_Data = pData;

		// Check if data storage was successful
		if (!pNode->p_Data)
		{
			// Handle errors
			printf("%s: %s\n", __FUNCTION__, "pNode->p_Data is null");
			exit(-1);
			return false;
		}
	}

	pNode->n_AllocationSize = nSize;		// Set allocation size
	pNode->s_Tag = sTag;			// Set tag id type
	pNode->e_Status = Used;			// Set status of node
	return true;
}

/* Free the node data and set flag to free
* @param pNode: Node to apply data to
* @param pData: New data to be added
* @param nSize: Size of the stack memory
* @return True if successful
*/
b8 sce::plfm::MemoryNodeList::DeallocateData(v8* pAddr)
{
	MemoryNode* p_head = nullptr;			// Head node

	// Set head node to beginning node
	p_head = m_sStartNode;

	// Cycle through the nodes
	while (p_head->p_Next)
	{
		// Test for address match
		if (p_head->p_Data == pAddr)
		{
			break;
		}

		// Set head node to next node
		p_head = p_head->p_Next;
	}

	// If the node is used delete
	if (p_head->e_Status == Used)
	{
		// Free data memory
		if (!VirtualFree(p_head->p_Data, NULL, MEM_RELEASE))
		{
			// Handle errors
			printf("%s: %s\n", __FUNCTION__, "failed to release (data) memory");
			exit(-1);

			return false;
		}

		p_head->p_Data		= nullptr;				// Set node data to null
		p_head->e_Status	= Free;					// Set node status to Free
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
const c16* sce::plfm::PlatformAllocator::m_szMutexName		= L"sce_plfm_alloc_mtx";			// Mutex name
HANDLE sce::plfm::PlatformAllocator::m_hMutex				= nullptr;							// Mutex handle
b8 sce::plfm::PlatformAllocator::m_bAllocInit				= false;							// Get/Set if class is initialised

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

	DWORD dw_mutexCode = 0;					// Mutex response code

	// Check for valid mutex
	if (m_hMutex)
	{
		// Wait for mutex here
		dw_mutexCode = WaitForSingleObject(m_hMutex, INFINITE);
	}
	
	// Check if Allocator is set up
	SetupAllocator();

	MemoryNode* p_node = nullptr;			// Head node

	// Scan for available node
	p_node = ScanAvailable();

	// Check if node is available
	if (!p_node)
	{
		// Handle errors
		wprintf(L"%s: %s\n", __FUNCTIONW__, L"p_node is null");
		exit(-1);
	}

	// Validate mutex code
	switch(dw_mutexCode)
	{ 
	case WAIT_ABANDONED:
	{
		// Handle errors
		wprintf(L"%s: %s\n", __FUNCTIONW__, L"Mutex abandoned event");
		exit(-1);
	}
	break;
	case WAIT_OBJECT_0:	
	{
		// Allocate new data memory address
		v8* p_data = VirtualAlloc(NULL, nSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		// Allocate new data to available node
		AllocateData(p_node, p_data, nSize, sTag);
	}
	break;
	case WAIT_TIMEOUT:
	{
		// Handle errors
		wprintf(L"%s: %s\n", __FUNCTIONW__, L"Mutex timed out event");
		exit(-1);
	}
	break;
	case WAIT_FAILED:
	{
		// Handle errors
		wprintf(L"%s: %s\n", __FUNCTIONW__, L"Mutex failed event");
		exit(-1);
	}
	break;
	}

	// Check for valid mutex
	if (m_hMutex)
	{
		// Release mutex
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

	DWORD dw_mutexCode = 0;					// Mutex response code

	// Check for valid mutex
	if (m_hMutex)
	{
		// Wait for mutex here
		dw_mutexCode = WaitForSingleObject(m_hMutex, INFINITE);
	}

	// Validate mutex code
	switch (dw_mutexCode)
	{
	case WAIT_ABANDONED:
	{
		// Handle errors
		wprintf(L"%s: %s\n", __FUNCTIONW__, L"Mutex abandoned");
		exit(-1);
	}break;
	case WAIT_OBJECT_0: {
		if (!pAddr)
		{
			// Handle errors
			wprintf(L"%s: %s\n", __FUNCTIONW__, L"pAddr is null");
			exit(-1);
		}

		// Deallocate data memory from storage node
		DeallocateData(pAddr);
	}break;
	case WAIT_TIMEOUT:
	{
		// Handle errors
		wprintf(L"%s: %s\n", __FUNCTIONW__, L"Mutex timed out");
		exit(-1);
	}break;
	case WAIT_FAILED:
	{
		// Handle errors
		wprintf(L"%s: %s\n", __FUNCTIONW__, L"Mutex failed");
		exit(-1);
	}break;
	}

	// Check for valid mutex
	if (m_hMutex)
	{
		// Release mutex
		ReleaseMutex(m_hMutex);
	}
}

/* Sets up the allocator class */
v8 sce::plfm::PlatformAllocator::SetupAllocator()
{
#ifdef SCE_PLATFORM_DEBUG
	wprintf(L"%s\n", __FUNCTIONW__);
#endif // !SCE_PLATFORM_DEBUG

	// Test if class is initilised
	if (!m_bAllocInit)
	{
		m_hMutex = CreateMutex(NULL, FALSE, m_szMutexName);		// Initalise mutext
		SetupList();											// Set up the NodeList

		m_bAllocInit = true;									// Update initalisation status
	}
}

/* Deallocatees all memory in the node list
*/
v8 sce::plfm::PlatformAllocator::ShutdownAllocator()
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

/* ==========================================================
* Platform class
* Manage the operatings system.
* Exported.
*/

/* Sets up the allocator class
*/
v8 sce::plfm::Platform::Setup()
{
#ifdef SCE_PLATFORM_DEBUG
	wprintf(L"%s\n", __FUNCTIONW__);
#endif // !SCE_PLATFORM_DEBUG
	
}

/* Shuts down the platform class
*/
v8 sce::plfm::Platform::Shutdown()
{
#ifdef SCE_PLATFORM_DEBUG
	wprintf(L"%s\n", __FUNCTIONW__);
#endif // !SCE_PLATFORM_DEBUG

	ShutdownAllocator();
}
#endif	// SCE_PLATFORM_UNICODE
#endif	// SCE_PLATFORM_WINDOWS

