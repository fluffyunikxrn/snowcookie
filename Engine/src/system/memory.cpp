/*
* ==================================================================
* File name:	Memory.h
* File desc:	Memory allocation management and operator new/delete
*				overloading.
* Author:		Chloe Tunrer (chloeturner@tuta.io)
* Copyright:	Copyright 2022 Chloe Tunrer
* ==================================================================
*/

/*
* ======================
* Include(s)
* ======================
*/
#include "pch.h"
#include "memory.h"

/*
* ===========================================================
* Override(s)
* -----------------------------------------------------------
* Overriding of the new/delete operator(s)
* -----------------------------------------------------------
*/
#pragma region newdelete
// Windows CRT new/delete
#ifdef SCE_PLATFORM_WINDOWS
// New overrides(s)
_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
v8* __CRTDECL operator new(size_t nSize)
{
	v8* p_mem = (void*)malloc(nSize);
	return sce::Memory::GetInstance()->NewMemory(p_mem, nSize, sce::MemoryTag::MEMORY_UNKNOWN);
}

_NODISCARD _Ret_maybenull_ _Success_(return != NULL) _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
v8* __CRTDECL operator new(size_t nSize, ::std::nothrow_t const&) noexcept
{
	v8* p_mem = (void*)malloc(nSize);
	return sce::Memory::GetInstance()->NewMemory(p_mem, nSize, sce::MemoryTag::MEMORY_UNKNOWN);
}

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
v8* __CRTDECL operator new[](size_t nSize)
{
	v8* p_mem = (void*)malloc(nSize);
	return sce::Memory::GetInstance()->NewMemory(p_mem, nSize, sce::MemoryTag::MEMORY_UNKNOWN);
}

_NODISCARD _Ret_maybenull_ _Success_(return != NULL) _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
v8* __CRTDECL operator new[](size_t nSize, ::std::nothrow_t const&) noexcept
{
	v8* p_mem = (void*)malloc(nSize);
	return sce::Memory::GetInstance()->NewMemory(p_mem, nSize, sce::MemoryTag::MEMORY_UNKNOWN);
}


// Delete overrides(s)
v8 __CRTDECL operator delete(v8 * pMem) noexcept
{
	return sce::Memory::GetInstance()->DeleteMemory(pMem);
}

v8 __CRTDECL operator delete(v8 * pMem, ::std::nothrow_t const&) noexcept
{
	return sce::Memory::GetInstance()->DeleteMemory(pMem);
}

v8 __CRTDECL operator delete[](v8* pMem) noexcept
{
	return sce::Memory::GetInstance()->DeleteMemory(pMem);
}

v8 __CRTDECL operator delete[](v8* pMem, ::std::nothrow_t const&) noexcept
{
	return sce::Memory::GetInstance()->DeleteMemory(pMem);
}

v8 __CRTDECL operator delete(v8 * pMem, size_t _Size) noexcept
{
	return sce::Memory::GetInstance()->DeleteMemory(pMem);
}

v8 __CRTDECL operator delete[](v8* pMem, size_t _Size) noexcept
{
	return sce::Memory::GetInstance()->DeleteMemory(pMem);
}
#endif // SCE_PLATFORM_WINDOWS

// Engine specific overrides
/* New operator with tag placement
* @param nSize:
* @param tag:
*/
v8* operator new(size_t nSize, sce::MemoryTag tag)
{
	v8* p_mem = (void*)malloc(nSize);
	return sce::Memory::GetInstance()->NewMemory(p_mem, nSize, tag);
}
#pragma endregion newdelete

/* ==========================================================
* MemoryNodes class
* -----------------------------------------------------------
* Manages the memory creation, manipulation and deletion
* and keeps the node list updated
* -----------------------------------------------------------
*/

/* Creates an empty new node, on the heap
*/
sce::MemoryNode* sce::MemoryNodes::NewNode()
{
	// Create node
	size64 n_size = sizeof(MemoryNode);
	MemoryNode* p_node = (MemoryNode*)malloc(n_size);

	if (p_node)
	{		
		p_node = (MemoryNode*)memset((v8*)p_node, 0, n_size);		// Set memory to 0
		p_node->bFree = true;										// Set node to unoccupied
		m_nLength++;												// Increase node list length

		// Update stats
		if (p_node)
		{			
			UpdateStats(UpdateMemoryStats::UPDATE_PLUS, n_size, MemoryTag::MEMORY_MEMORYNODE);
		}
	}

	return p_node;
}

/* Validates and deletes a specific node
* * @param pNode: memory to delete
*/
v8 sce::MemoryNodes::DeleteNode(MemoryNode* pNode)
{	
	MemoryNode* head = m_StartNode;		// Set head node
	MemoryNode* next = nullptr;			// Set next node
	MemoryNode* prev = nullptr;			// Set previous node

	// Check if head node and node to be deleted are the same
	if (head == pNode)
	{
		if (head)
		{
			// Delete any left over data
			if (head->pData && !head->bFree)
			{
				sce::Memory::GetInstance()->DeleteMemory(head->pData);
			}

			next = head->pNext;				// Set head node

			// Delete node
			free(head);
			head = nullptr;
			m_StartNode = nullptr;
			m_nLength--;

			if (next)
			{
				m_StartNode = next;			// Set next node
			}
		}

		// Update stats
		if (!head)
		{			
			UpdateStats(UpdateMemoryStats::UPDATE_MINUS, sizeof(MemoryNode), MemoryTag::MEMORY_MEMORYNODE);
		}
	}

	if (head)
	{
		while (head->pNext)
		{
			next = head->pNext;					// Set next node
			prev = head;						// Set previous node

			if (next == pNode)
			{
				prev->pNext = next->pNext;			// Set previous node (next node) to next node (next node)

				// Delete any left over data
				if (next->pData && !next->bFree)
				{
					sce::Memory::GetInstance()->DeleteMemory(head->pData);
				}

				// Delete node
				free(next);
				next = nullptr;
				m_nLength--;

				// Update stats
				if (!next)
				{
					UpdateStats(UpdateMemoryStats::UPDATE_MINUS, sizeof(MemoryNode), MemoryTag::MEMORY_MEMORYNODE);
				}

				break;
			}

			head = next;		// Set head node
		}
	}
}

/* Shuts down all nodes, must be called after ShutdownMemory
*/
v8 sce::MemoryNodes::ShutdownNodes()
{	
	MemoryNode* head = m_StartNode;		// Set head node
	MemoryNode* next = nullptr;			// Set next node

	if (head)
	{
		// Cycle thorugh node allocations
		while (head->pNext)
		{
			next = head->pNext;			// Set next node to head node (next node)

			// Check for and remove node allocation
			DeleteNode(head);

			head = head->pNext;			// Set head node
		}

		// Check for and remove last node allocation
		if (head)
		{
			DeleteNode(head);
		}
	}
}

v8 sce::MemoryNodes::UpdateStats(UpdateMemoryStats direction, size64 nSize, MemoryTag tag)
{
	if (direction == UpdateMemoryStats::UPDATE_PLUS)
	{
		size64 n_size = sizeof(void*);
		m_nTotalPointerMemoryUsed += n_size;
		m_nTotalMemoryAllocated += nSize;
		m_nTotalPointerByTag[tag] += n_size;
		m_nTotalAllocatedByTag[tag] += nSize;
	}
	else if (direction == UpdateMemoryStats::UPDATE_MINUS)
	{
		size64 n_size = sizeof(void*);
		m_nTotalPointerMemoryUsed -= n_size;
		m_nTotalMemoryAllocated -= nSize;
		m_nTotalPointerByTag[tag] -= n_size;
		m_nTotalAllocatedByTag[tag] -= nSize;
	}
}

/* ==========================================================
* Memory class
* -----------------------------------------------------------
* Creates a static singleton to managte memory creation,
* tracking and statistics
* -----------------------------------------------------------
*/

// Static initalisation
sce::Memory* sce::Memory::g_pInstance = (Memory*)malloc(sizeof(Memory));			// Memory class instance
size64 sce::Memory::m_nLength = 0;													// Node list length
size64 sce::Memory::m_nTotalPointerMemoryUsed = 0;										// Total pointer memory used
size64 sce::Memory::m_nTotalMemoryAllocated = 0;											// Total allocated memory used
size64 sce::Memory::m_nTotalPointerByTag[sce::MemoryTag::MEMORY_MAXMEMORYTAGS] = {};		// Total pointer memory by tag type used
size64 sce::Memory::m_nTotalAllocatedByTag[sce::MemoryTag::MEMORY_MAXMEMORYTAGS] = {};	// Total memory allocated by tag type used

/* MemoryNode
*
* Creates multiple nodes specified by length
* @param nLength: memory to be deleted
* @return: heap memory
*/
sce::MemoryNode* sce::Memory::CreateNodePool(size64 nLength)
{
	return nullptr;
}

// Method
/* New
* 
* Creates statistics about the new memory object on the heap
* 
* @param pMem: memory to processed
* @param nSize: size of allocation
* @param Tag: set the memory type
* @return: heap memory
*/
v8* sce::Memory::NewMemory(v8* pMem, size64 nSize, MemoryTag tag)
{
	// Create initial node if does not exist
	if (!GetInstance()->m_StartNode)
	{
		GetInstance()->m_StartNode = GetInstance()->NewNode();
	}

	// Create node to be added
	MemoryNode node;
	memset(&node, 0, sizeof(MemoryNode));
	node.bFree = true;
	node.nAllocationSize = nSize;
	node.nPointerSize = sizeof(void*);
	node.pData = pMem;
	node.pNext = nullptr;
	node.Tag = tag;

	// Set head node
	MemoryNode* head = sce::Memory::GetInstance()->m_StartNode;

 	if (head)
	{
		if (!head->pData && head->bFree)
		{
			memcpy(head, &node, sizeof(MemoryNode));		// Copy memory to node list
			head->bFree = false;							// Set memory node to occupied

			// Update stats
			if (!head->bFree)
			{
				GetInstance()->UpdateStats(UpdateMemoryStats::UPDATE_PLUS, nSize, tag);
			}
		}
		else if (head->pData && !head->bFree)
		{
			// Cycle thorugh memory allocations
			while (head->pNext)
			{
				// Set head to next node
				head = head->pNext;
			}

			// Create new node
			if (!head->pNext)
			{
				head->pNext = sce::Memory::GetInstance()->NewNode();
			}

			if (head->pNext)
			{				
				memcpy(head->pNext, &node, sizeof(MemoryNode));		// Copy memory to node list
				head->pNext->bFree = false;							// Set memory node to occupied

				// Update stats
				if (!head->pNext->bFree)
				{
					GetInstance()->UpdateStats(UpdateMemoryStats::UPDATE_PLUS, nSize, tag);
				}				
			}			
		}
	}

	return pMem;
}

/* Delete
* -----------------------------------------------------------
* Deletes memory at specific address
* -----------------------------------------------------------
* @param pMem: memory to be deleted
* @return: heap memory
*/
v8 sce::Memory::DeleteMemory(v8* pMem)
{
	MemoryNode* head = GetInstance()->m_StartNode;		// Set head node
	MemoryNode* next = nullptr;							// Set next node

	if (head)
	{
		// Cycle thorugh memory allocations
		while (head->pNext)
		{
			next = head->pNext;				// Set next node

			if (head->pData == pMem && !head->bFree)
			{
				size64 n_size = head->nAllocationSize;			// Store allocation size
				MemoryTag t_tag = head->Tag;					// Store tag type

				// Delete memory
				free(head->pData);
				head->pData = nullptr;
				head->bFree = true;
				pMem = nullptr;

				// Update stats
				if (head->bFree)
				{
					GetInstance()->UpdateStats(UpdateMemoryStats::UPDATE_MINUS, head->nAllocationSize, head->Tag);
				}

				break;
			}

			head = next;			// Set head node
		}

		if (head->pData == pMem && !head->bFree)
		{
			size64 n_size = head->nAllocationSize;
			MemoryTag t_tag = head->Tag;			

			// Delete memory
			free(head->pData);
			head->pData = nullptr;
			head->bFree = true;
			pMem = nullptr;

			// Update stats
			if (head->bFree)
			{
				GetInstance()->UpdateStats(UpdateMemoryStats::UPDATE_MINUS, head->nAllocationSize, head->Tag);
			}
		}
	}	
}

/* ShutdownMemory
* -----------------------------------------------------------
* Shuts down all active pointers stared in nodes
* -----------------------------------------------------------
*/
v8 sce::Memory::ShutdownMemory()
{	
	MemoryNode* head = GetInstance()->m_StartNode;		// Set head node
	MemoryNode* next = nullptr;							// Create next node

	// Cycle thorugh memory allocations
	while (head->pNext)
	{
		next = head->pNext;		// Set next node

		// Check for and remove memory allocation
		if (head->pData && !head->bFree)
		{
			// Delete memory allocation
			DeleteMemory(head->pData);
		}

		head = next;			// Set head node
	}

	// Check for and remove last memory allocation
	if (head)
	{
		if (head->pData && !head->bFree)
		{
			// Delete memory allocation
			DeleteMemory(head->pData);
		}
	}

	// Shutodwn nodes
	GetInstance()->ShutdownNodes();
}
