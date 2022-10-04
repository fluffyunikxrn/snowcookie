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
* Defined in memory.h.
* @param nSize: size of stack memory
* @param sId: Identifying tag
* @return Newly aquired memory address
*/
v8* operator new(size_t nSize, sce::MemoryFlag sFlag)
{
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	return sce::Platform::GetInstance().Allocate(nSize, sFlag);
}

/*
* ===========================================================
* Platform: Windows
* ===========================================================
*/
#if defined(SCE_PLATFORM_WINDOWS)

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
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	return sce::Platform::GetInstance().Allocate(nSize, sce::MemoryFlag::MEMORY_UNKNOWN);
}

_NODISCARD _Ret_maybenull_ _Success_(return != NULL) _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
v8* __CRTDECL operator new(size_t nSize, ::std::nothrow_t const&) noexcept
{
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	return sce::Platform::GetInstance().Allocate(nSize, sce::MemoryFlag::MEMORY_UNKNOWN);
}

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
v8* __CRTDECL operator new[](size_t nSize)
{
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	return sce::Platform::GetInstance().Allocate(nSize, sce::MemoryFlag::MEMORY_UNKNOWN);
}

_NODISCARD _Ret_maybenull_ _Success_(return != NULL) _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
v8* __CRTDECL operator new[](size_t nSize, ::std::nothrow_t const&) noexcept
{
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	return sce::Platform::GetInstance().Allocate(nSize, sce::MemoryFlag::MEMORY_UNKNOWN);
}

// Delete overrides(s)
v8 __CRTDECL operator delete(v8 * pAddr) noexcept
{
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	sce::Platform::GetInstance().Deallocate(pAddr);
}

v8 __CRTDECL operator delete(v8 * pAddr, ::std::nothrow_t const&) noexcept
{
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	sce::Platform::GetInstance().Deallocate(pAddr);
}

v8 __CRTDECL operator delete[](v8* pAddr) noexcept
{
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	sce::Platform::GetInstance().Deallocate(pAddr);
}

v8 __CRTDECL operator delete[](v8* pAddr, ::std::nothrow_t const&) noexcept
{
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	sce::Platform::GetInstance().Deallocate(pAddr);
}

v8 __CRTDECL operator delete(v8 * pAddr, size_t _Size) noexcept
{
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	sce::Platform::GetInstance().Deallocate(pAddr);
}

v8 __CRTDECL operator delete[](v8* pAddr, size_t _Size) noexcept
{
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	sce::Platform::GetInstance().Deallocate(pAddr);
}
#pragma endregion newdelete

/*
* ===========================================================
* Platform: Unicode
* ===========================================================
*/
#if defined(SCE_PLATFORM_UNICODE)
/* ==========================================================
* PlatformAllocator class
* Manages memory allocation globally for operating system.
* Thread safe.
* Not exported.
*/

/*
* ===========================================================
* Static initialisation(s)
* ===========================================================
*/
LPCWSTR sce::sys::PlatformAllocator::m_szMutexName					= L"win_uc_alloc_mutex";
HANDLE sce::sys::PlatformAllocator::m_hMutex						= CreateMutex(NULL, FALSE, m_szMutexName);
sce::sys::MemoryNode* sce::sys::PlatformAllocator::m_sStartNode		= nullptr;
size64 sce::sys::PlatformAllocator::m_uLength						= 0;
sce::sys::MemoryNodeStats sce::sys::PlatformAllocator::m_sStats		= {};

/* Creates a new node and adds to node list
*/
sce::sys::MemoryNode* sce::sys::PlatformAllocator::CreateNode(MemoryNode* pPrev)
{
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	MemoryNode* p_head = nullptr;
	MemoryNode* p_node = static_cast<MemoryNode*>(VirtualAlloc(NULL, sizeof(MemoryNode), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));

	if (!p_node)
	{
		printf("%s: %s\n", __FUNCTION__, "failed to MemoryNode");
		exit(-1);
	}

	//p_node->e_Status			= Free;
	p_node->n_AllocationSize	= 0;
	p_node->n_NodeSize			= sizeof(MemoryNode);
	p_node->p_Data				= nullptr;
	p_node->p_Next				= nullptr;

	if (pPrev)
	{
		p_node->p_Prev = pPrev;
	}
	else
	{
		p_node->p_Prev = nullptr;
	}

	p_node->s_Flag				= MemoryFlag::MEMORY_EMPTY;
	m_uLength++;

	m_sStats.TotalPointerMemoryUsed									+= sizeof(v8*);
	m_sStats.FlagPointerMemoryUsed[MemoryFlag::MEMORY_MEMNODE]		+= sizeof(v8*);
	m_sStats.TotalAllocationMemoryUsed								+= sizeof(MemoryNode);
	m_sStats.FlagAllocationMemoryUsed[MemoryFlag::MEMORY_MEMNODE]	+= sizeof(MemoryNode);

	return p_node;
}

/* Deletes a specific node, joins the list up and
* can be used to force the delete to take place, if
* the node isn't free. This will also release the
* data stored.
* @param pNode: Node to be processed
* @param bForce: Indicate whether to force delete
* @return True if succeeded
*/
v8 sce::sys::PlatformAllocator::DeleteNode(MemoryNode* pNode, b8 bForce)
{
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	DWORD dw_mutexResult	= 0;
	MemoryNode* p_proc		= nullptr;
	MemoryNode* p_next		= nullptr;
	MemoryNode* p_prev		= nullptr;
	b8 b_start				= false;

	if (m_hMutex)
	{
		dw_mutexResult = WaitForSingleObject(m_hMutex, INFINITE);
	}

	p_proc = pNode;

	if (!p_proc)
	{
		printf("%s: %s\n", __FUNCTION__, "processing node is null");
		exit(-1);
	}

	if (p_proc == m_sStartNode)
	{
		b_start = true;
	}
	else
	{
		// TODO: Code needs validating, if not start node and has previous node
		if (p_proc->p_Prev)
		{
			p_prev = p_proc->p_Prev;
		}
	}

	if (p_proc->p_Next)
	{
		p_next = p_proc->p_Next;
	}

	//if (bForce && p_proc->p_Data && p_head->s_Flag == MemoryFlag::MEMORY_EMPTY)
	if (bForce && p_proc->p_Data && p_proc->s_Flag != MemoryFlag::MEMORY_EMPTY)
	{
		p_proc = ReleaseNode(p_proc);
	}

	if (!VirtualFree(p_proc, NULL, MEM_RELEASE))
	{
		printf("%s: %s %d\n", __FUNCTION__, "VirtualFree:", GetLastError());
		exit(-1);
	}

	p_proc = nullptr;

	if (b_start)
	{
		m_sStartNode = p_next;

		if (m_sStartNode)
		{
			m_sStartNode->p_Prev = nullptr;
		}
	}
	else
	{
		// TODO: Code needs validating, if not start node and has previous node
		if (p_prev)
		{
			p_prev->p_Next = p_next;
		}
	}

	m_sStats.TotalPointerMemoryUsed									-= sizeof(v8*);
	m_sStats.FlagPointerMemoryUsed[MemoryFlag::MEMORY_MEMNODE]		-= sizeof(v8*);
	m_sStats.TotalAllocationMemoryUsed								-= sizeof(MemoryNode);
	m_sStats.FlagAllocationMemoryUsed[MemoryFlag::MEMORY_MEMNODE]	-= sizeof(MemoryNode);

	if (m_hMutex)
	{
		dw_mutexResult = ReleaseMutex(m_hMutex);
	}
}

/* Commit memory and statistics to free or new node
* @param pNode: Node to be processed
*/
sce::sys::MemoryNode* sce::sys::PlatformAllocator::CommitNode(MemoryNode* pNode, size64 nSize, MemoryFlag sFlag)
{
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	MemoryNode* p_proc = nullptr;

	p_proc = pNode;	

	if (!p_proc)
	{
		printf("%s: %s\n", __FUNCTION__, "head is null");
		exit(-1);
	}

	if (!(p_proc->p_Data = VirtualAlloc(NULL, nSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE)))
	{
		printf("%s: %s %d\n", __FUNCTION__, "VirtualAlloc:", GetLastError());
		exit(-1);
	}

	p_proc->n_AllocationSize	= nSize;
	p_proc->s_Flag				= sFlag;

	m_sStats.TotalPointerMemoryUsed				+= sizeof(v8*);
	m_sStats.FlagPointerMemoryUsed[sFlag]		+= sizeof(v8*);
	m_sStats.TotalAllocationMemoryUsed			+= nSize;
	m_sStats.FlagAllocationMemoryUsed[sFlag]	+= nSize;

//#ifdef SCE_PLATFORM_DEBUG
//	const c16* print = NodeToString(p_proc);
//	wprintf(L"%s\n", print);
//#endif // !SCE_PLATFORM_DEBUG

	return p_proc;
}

/* Release memory from previously commited node
* @param pNode: Node to be processed
*/
sce::sys::MemoryNode* sce::sys::PlatformAllocator::ReleaseNode(MemoryNode* pNode, size64 nSize)
{
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	MemoryNode* p_proc	= nullptr;

	MemoryFlag s_flag = MemoryFlag::MEMORY_EMPTY;
	size64 n_size = 0;
	
	p_proc = pNode;

	if (!p_proc)
	{
		printf("%s: %s\n", __FUNCTION__, "process node is null");
		exit(-1);
	}

	if (p_proc->p_Data || p_proc->s_Flag != MemoryFlag::MEMORY_EMPTY)
	{
		if (!p_proc->p_Data)
		{
			printf("%s: %s\n", __FUNCTION__, "data is null");
			exit(-1);
		}

		s_flag = p_proc->s_Flag;
		n_size = p_proc->n_AllocationSize;

		if (!VirtualFree(p_proc->p_Data, NULL, MEM_RELEASE))
		{
			printf("%s: %s %d\n", __FUNCTION__, "VirtualFree:", GetLastError());
			exit(-1);
		}

		//p_proc->e_Status			= Free;
		p_proc->n_AllocationSize = 0;
		p_proc->p_Data = nullptr;
		p_proc->s_Flag = MemoryFlag::MEMORY_EMPTY;
	}

	m_sStats.TotalPointerMemoryUsed				-= sizeof(v8*);
	m_sStats.FlagPointerMemoryUsed[s_flag]		-= sizeof(v8*);
	m_sStats.TotalAllocationMemoryUsed			-= n_size;
	m_sStats.FlagAllocationMemoryUsed[s_flag]	-= n_size;

//#ifdef SCE_PLATFORM_DEBUG
//	const c16* print = NodeToString(p_proc);
//	wprintf(L"%s\n", print);
//#endif // !SCE_PLATFORM_DEBUG

	return p_proc;
}

/* Scan for a free node or create new one and link it
* @return Available or free node
*/
sce::sys::MemoryNode* sce::sys::PlatformAllocator::ScanAvailable()
{
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	MemoryNode* p_proc = nullptr;
	MemoryNode* p_head = nullptr;

	if (!m_sStartNode)
	{
		m_sStartNode = p_proc = CreateNode();
		return p_proc;
	}

	p_head = m_sStartNode;

	while (p_head->p_Next)
	{
		p_head = p_head->p_Next;
	}

	if (p_head->p_Data == nullptr && p_head->s_Flag == MemoryFlag::MEMORY_EMPTY)
	{
		p_proc = p_head;

		if (!p_proc)
		{
			printf("%s: %s\n", __FUNCTION__, "process node is null");
			exit(-1);
		}

		return p_proc;
	}

	if (p_head->p_Next == nullptr)
	{
		p_head->p_Next = CreateNode(p_head);
		p_proc = p_head->p_Next;

		if (!p_proc)
		{
			printf("%s: %s\n", __FUNCTION__, "process node is null");
			exit(-1);
		}

		return p_proc;
	}

	return nullptr;
}

/* Finds a specific node by address
* @param pAddr: address of pointer/node to find
* @return Corresponding node, if no node found null
*/
sce::sys::MemoryNode* sce::sys::PlatformAllocator::FindNode(v8* pAddr)
{
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	MemoryNode* p_proc	= nullptr;
	MemoryNode* p_head	= nullptr;
	v8* p_addr			= nullptr;

	p_head = m_sStartNode;
	p_addr = pAddr;

	if (!p_head)
	{
		printf("%s: %s\n", __FUNCTION__, "head node is null");
		exit(-1);
	}

	if (!p_addr)
	{
		printf("%s: %s\n", __FUNCTION__, "address is null");
		exit(-1);
	}

	while (p_head)
	{
		if (p_head->p_Data == p_addr)
		{
			p_proc = p_head;
			break;
		}

		p_head = p_head->p_Next;
	}

	if (!p_proc)
	{
		printf("%s: %s\n", __FUNCTION__, "processing node is null");
		exit(-1);
	}

	return p_proc;
}

/* Shut down all memory allocated throughout runtime
*/
v8 sce::sys::PlatformAllocator::ShutdownAllocator()
{
#ifdef SCE_PLATFORM_DEBUG
	printf("%s\n", __FUNCTION__);
#endif // !SCE_PLATFORM_DEBUG

	DWORD dw_mutexResult	= 0;
	MemoryNode* p_head		= nullptr;
	MemoryNode* p_next		= nullptr;

	p_head = m_sStartNode;

	while (p_head)
	{
		p_next = p_head->p_Next;

		DeleteNode(p_head, true);

		p_head = p_next;
	}
}

/* Allocates new memory on heap and returns it
* @param nSize: Size of allocation on stack
* @param sFlag: Identifying name
* @return address of newly created memory
*/
v8* sce::sys::PlatformAllocator::Allocate(size64 nSize, MemoryFlag sFlag)
{
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	DWORD dw_mutexResult	= 0;
	MemoryNode* p_proc		= nullptr;
	MemoryNode* p_head		= nullptr;

	if (m_hMutex)
	{
		dw_mutexResult = WaitForSingleObject(m_hMutex, INFINITE);
	}

	switch (dw_mutexResult)
	{
	case WAIT_ABANDONED:
	{
		printf("%s: %s\n", __FUNCTION__, "mutex abandoned");
		exit(-1);
	} break;
	case WAIT_OBJECT_0:
	{
		p_head = ScanAvailable();

		if (!p_head)
		{
			printf("%s: %s\n", __FUNCTION__, "mutex ");
			exit(-1);
		}

		p_proc = CommitNode(p_head, nSize, sFlag);		
	} break;
	case WAIT_TIMEOUT:
	{
		printf("%s: %s\n", __FUNCTION__, "mutex timed out");
		exit(-1);
	} break;
	case WAIT_FAILED:
	{
		printf("%s: %s\n", __FUNCTION__, "mutex failed");
		exit(-1);
	} break;
	}

	if (m_hMutex)
	{
		dw_mutexResult = ReleaseMutex(m_hMutex);
	}

	if (!p_proc)
	{
		printf("%s: %s\n", __FUNCTION__, "processing node is null");
		exit(-1);
	}

	if (!p_proc->p_Data)
	{
		printf("%s: %s\n", __FUNCTION__, "processing (Data) node is null");
		exit(-1);
	}

	return p_proc->p_Data;
}

/* Deallocates previous allocated memory at specified
* address
* @param pAddr: address to delete
*/
v8 sce::sys::PlatformAllocator::Deallocate(v8* pAddr)
{
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	DWORD dw_mutexResult	= 0;
	MemoryNode* p_proc		= nullptr;
	v8* p_addr				= nullptr;

	if (m_hMutex)
	{
		dw_mutexResult = WaitForSingleObject(m_hMutex, INFINITE);
	}

	p_addr = pAddr;

	if (!p_addr)
	{
		printf("%s: %s\n", __FUNCTION__, "address is null");
		exit(-1);
	}

	p_proc = FindNode(p_addr);

	if (!p_proc)
	{
		printf("%s: %s\n", __FUNCTION__, "processing node is null");
		exit(-1);
	}

	ReleaseNode(p_proc);

	if (m_hMutex)
	{
		dw_mutexResult = ReleaseMutex(m_hMutex);
	}
}

/* Retrieves a specific nodes starts to a string
* @param pNode: Node to find statistics for
* @return String containing the node statistics
*/
const c16* sce::sys::PlatformAllocator::NodeToString(MemoryNode* pNode) 
{
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	const u16 n_bufLen = 1024;
	MemoryNode* p_proc = nullptr;	
	c16* p_out			= nullptr;		
	size64 n_len		= 0;	

	c8 p_buffer[n_bufLen];
	c16 p_copy[n_bufLen];

	p_proc = pNode;

	if (!p_proc)
	{
		printf("%s: %s\n", __FUNCTION__, "processing node is null");
		exit(-1);
	}

	memset(p_buffer, 0, sizeof(c8));
	wmemset(p_copy, 0, sizeof(c16));

	//n_len = snprintf(p_buffer + n_len, n_bufLen, "Node Stats (%p):\n", p_proc->s_Flag);
	n_len = snprintf(p_buffer + n_len, n_bufLen, "Node Stats (%s):\n", sce_NodeStatusToStringA(p_proc->s_Flag));

	n_len += snprintf(p_buffer + n_len, n_bufLen, "\t%s:\t%p\n", "Node address", p_proc);
	n_len += snprintf(p_buffer + n_len, n_bufLen, "\t%s:\t%.2f bytes\n", "Node size", static_cast<f32>(p_proc->n_NodeSize));

	c8* sz_tagStr = const_cast<c8*>(sce_MemoryTagToStringA(p_proc->s_Flag));

	n_len += snprintf(p_buffer + n_len, n_bufLen, "\t%s:\t%s\n", "Memory Tag", sz_tagStr);
	
	n_len += snprintf(p_buffer + n_len, n_bufLen, "\t%s:\t%p\n", "Data address", p_proc->p_Data);
	n_len += snprintf(p_buffer + n_len, n_bufLen, "\t%s:\t%.2f\n", "Data size", static_cast<f32>(p_proc->n_AllocationSize));
	n_len += snprintf(p_buffer + n_len, n_bufLen, "\t%s:\t%p\n", "Previous node", p_proc->p_Prev);
	n_len += snprintf(p_buffer + n_len, n_bufLen, "\t%s:\t%p", "Next node", p_proc->p_Next);

	n_len = mbstowcs(p_copy, p_buffer, n_len + 1);
	
	if (!p_copy)
	{
		printf("%s: %s\n", __FUNCTION__, "copy buffer is null");
		exit(-1);
	}

	p_out = _wcsdup(p_copy);
	return p_out;
}

/* Gets the size of the allocation on stack for input
* address
* @param pAddr: Address of tyhe memory
* @return Size of the alocation
*/
size64 sce::sys::PlatformAllocator::AddressStackSize(v8* pAddr)
{
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	DWORD dw_mutexResult	= 0;
	MemoryNode* p_proc		= nullptr;
	MemoryNode* p_head		= nullptr;

	if (m_hMutex)
	{
		dw_mutexResult = WaitForSingleObject(m_hMutex, INFINITE);
	}

	p_head = FindNode(pAddr);	

	if (!p_head)
	{
		printf("%s: %s\n", __FUNCTION__, "head node is null");
		exit(-1);
	}

	p_proc = p_head;

	if (m_hMutex)
	{
		dw_mutexResult = ReleaseMutex(m_hMutex);
	}

	if (p_proc)
	{
		return p_proc->n_AllocationSize;
	}

	return 0;
}

/* Get the memory statistics
* @return Memory statistics
*/
sce::sys::MemoryNodeStats sce::sys::PlatformAllocator::GetStats()
{
	return m_sStats;
}

/* Returns a string containing the stats for total
* memory used
*/
const c16* sce::sys::PlatformAllocator::StatsToString()
{
//#ifdef SCE_PLATFORM_DEBUG
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	const u16 n_bufferSize			= 8012;
	const u16 n_arraySize			= 1024;
	u16 n_length					= 0;
	u16 n_count						= 0;
	MemorySizeResult* s_result		= nullptr;
	c8* sz_aBuffer					= nullptr;
	c16* sz_wBuffer					= nullptr;
	c16* sz_out						= nullptr;

	if (!(sz_aBuffer = static_cast<c8*>(VirtualAlloc(NULL, sizeof(c8) * n_bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE))))
	{
		printf("%s: VirtualAlloc (%d)\n", __FUNCTION__, GetLastError());
		return nullptr;
	}

	if (!(sz_wBuffer = static_cast<c16*>(VirtualAlloc(NULL, sizeof(c16) * n_bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE))))
	{
		printf("%s: VirtualAlloc (%d)\n", __FUNCTION__, GetLastError());
		return nullptr;
	}

	if (!(s_result = static_cast<MemorySizeResult*>(VirtualAlloc(NULL, sizeof(MemorySizeResult) * n_arraySize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE))))
	{
		printf("%s: VirtualAlloc (%d)\n", __FUNCTION__, GetLastError());
		return nullptr;
	}

	s_result[n_count] = sce_MemorySizeTest(m_sStats.TotalPointerMemoryUsed, MemoryDecBin::Binary);
	n_length = snprintf(sz_aBuffer + n_length, n_bufferSize, "Total heap memory used: %.02lf %s\n", s_result[n_count].n_Size, s_result[n_count].ch_Unit);

	for (int i = 0; i < MemoryFlag::MEMORY_MAXTAGS; i++)
	{
		const char* name = sce_MemoryTagToStringA(static_cast<MemoryFlag>(i));
		s_result[++n_count] = sce_MemorySizeTest(m_sStats.FlagPointerMemoryUsed[i], MemoryDecBin::Binary);

		if (15 < strlen(name))
		{
			n_length += snprintf(sz_aBuffer + n_length, n_bufferSize, "\t%s:\t%.02lf %s\n", name, s_result[n_count].n_Size, s_result[n_count].ch_Unit);
		}
		else
		{
			n_length += snprintf(sz_aBuffer + n_length, n_bufferSize, "\t%s:\t\t%.02lf %s\n", name, s_result[n_count].n_Size, s_result[n_count].ch_Unit);
		}		
	}

	s_result[++n_count] = sce_MemorySizeTest(m_sStats.TotalAllocationMemoryUsed, MemoryDecBin::Binary);
	n_length += snprintf(sz_aBuffer + n_length, n_bufferSize, "Total stack memory used: %.02lf %s\n", s_result[n_count].n_Size, s_result[n_count].ch_Unit);

	for (int i = 0; i < MemoryFlag::MEMORY_MAXTAGS; i++)
	{
		const char* name = sce_MemoryTagToStringA(static_cast<MemoryFlag>(i));
		s_result[++n_count] = sce_MemorySizeTest(m_sStats.FlagAllocationMemoryUsed[i], MemoryDecBin::Binary);

		if (15 < strlen(name))
		{
			n_length += snprintf(sz_aBuffer + n_length, n_bufferSize, "\t%s:\t%.02lf %s\n", name, s_result[n_count].n_Size, s_result[n_count].ch_Unit);
		}
		else
		{
			n_length += snprintf(sz_aBuffer + n_length, n_bufferSize, "\t%s:\t\t%.02lf %s\n", name, s_result[n_count].n_Size, s_result[n_count].ch_Unit);
		}
	}

	if (!(VirtualFree(s_result, NULL, MEM_RELEASE)))
	{
		printf("%s: VirtualFree (%d)\n", __FUNCTION__, GetLastError());
		return nullptr;
	}

	s_result	= nullptr;

	if ('\n' == sz_aBuffer[strlen(sz_aBuffer) - 1])
	{
		sz_aBuffer[strlen(sz_aBuffer) - 1] = '\0';
	}

	mbstowcs(sz_wBuffer, sz_aBuffer, n_length + 1);

	if (!(VirtualFree(sz_aBuffer, NULL, MEM_RELEASE)))
	{
		printf("%s: VirtualFree (%d)\n", __FUNCTION__, GetLastError());
		return nullptr;
	}

	sz_out = _wcsdup(sz_wBuffer);

	if (!(VirtualFree(sz_wBuffer, NULL, MEM_RELEASE)))
	{
		printf("%s: VirtualFree (%d)\n", __FUNCTION__, GetLastError());
		return nullptr;
	}	

	sz_aBuffer	= nullptr;
	s_result	= nullptr;

	return sz_out;
}

/* ==========================================================
* Platform class
* Manage the operating system.
* Exported.
*/
v8 sce::Platform::Shutdown()
{
#ifdef SCE_PLATFORM_DEBUG
	printf("%s\n", __FUNCTION__);
#endif // !SCE_PLATFORM_DEBUG
	
	ShutdownAllocator();
}

#endif	// SCE_PLATFORM_UNICODE
#endif	// SCE_PLATFORM_WINDOWS
