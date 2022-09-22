#include "pch.h"
#include "memory.h"

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
void* __CRTDECL operator new(size_t nSize)
{
#if defined(SCE_PLATFORM_DEBUG)
	std::wcout << __FUNCTIONW__ << std::endl;
#endif

	void* p_mem = (void*)malloc(nSize);
	return sce::Memory::GetInstance()->New(p_mem, nSize, sce::MemoryTag::MEMORY_UNKNOWN);
}

_NODISCARD _Ret_maybenull_ _Success_(return != NULL) _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
void* __CRTDECL operator new(size_t nSize, ::std::nothrow_t const&) noexcept
{
#if defined(SCE_PLATFORM_DEBUG)
	std::wcout << __FUNCTIONW__ << std::endl;
#endif

	void* p_mem = (void*)malloc(nSize);
	return sce::Memory::GetInstance()->New(p_mem, nSize, sce::MemoryTag::MEMORY_UNKNOWN);
}

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
void* __CRTDECL operator new[](size_t nSize)
{
#if defined(SCE_PLATFORM_DEBUG)
	std::wcout << __FUNCTIONW__ << std::endl;
#endif

	void* p_mem = (void*)malloc(nSize * sizeof(void*));
	return sce::Memory::GetInstance()->New(p_mem, nSize, sce::MemoryTag::MEMORY_UNKNOWN);
}

_NODISCARD _Ret_maybenull_ _Success_(return != NULL) _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
void* __CRTDECL operator new[](size_t nSize, ::std::nothrow_t const&) noexcept
{
#if defined(SCE_PLATFORM_DEBUG)
	std::wcout << __FUNCTIONW__ << std::endl;
#endif

	return 0;
}

void __CRTDECL operator delete(void* pMem) noexcept
{
#if defined(SCE_PLATFORM_DEBUG)
	std::wcout << __FUNCTIONW__ << std::endl;
#endif

	sce::Memory::GetInstance()->Delete(pMem, sizeof(pMem), sce::MEMORY_UNKNOWN);
}

void __CRTDECL operator delete(void* pMem, ::std::nothrow_t const&) noexcept
{
#if defined(SCE_PLATFORM_DEBUG)
	std::wcout << __FUNCTIONW__ << std::endl;
#endif

}

void __CRTDECL operator delete[](void* pMem) noexcept
{
#if defined(SCE_PLATFORM_DEBUG)
	std::wcout << __FUNCTIONW__ << std::endl;
#endif

}

void __CRTDECL operator delete[](void* pMem, ::std::nothrow_t const&) noexcept
{
#if defined(SCE_PLATFORM_DEBUG)
	std::wcout << __FUNCTIONW__ << std::endl;
#endif

}

void __CRTDECL operator delete(void* pMem, size_t nSize) noexcept
{
#if defined(SCE_PLATFORM_DEBUG)
	std::wcout << __FUNCTIONW__ << std::endl;
#endif

	sce::Memory::GetInstance()->Delete(pMem, nSize, sce::MEMORY_UNKNOWN);
}

void __CRTDECL operator delete[](void* pMem, size_t nSize) noexcept
{
#if defined(SCE_PLATFORM_DEBUG)
	std::wcout << __FUNCTIONW__ << std::endl;
#endif

}

// Create Memory instance
sce::Memory* sce::Memory::g_pInstance = (sce::Memory*)malloc(sizeof(sce::Memory));

// Static initialisation
size64 sce::Memory::m_TotalPointerMemory = 0;
size64 sce::Memory::m_TotalAllocatedMemory = 0;
size64 sce::Memory::m_TaggedAllocations[sce::MemoryTag::MEMORY_MAXMEMORYTAGS];
sce::PointerBlock sce::Memory::m_PointerInfo[g_nMaxPointers];
u64 sce::Memory::m_nPointerCount = 0;
const char* const sce::Memory::m_StringArray[sce::MemoryTag::MEMORY_MAXMEMORYTAGS] = {
	"MEMORY_UNKNOWN",				// 0
	"MEMORY_INTEGER8",				// 1
	"MEMORY_INTEGER16",				// 2
	"MEMORY_INTEGER32",				// 3
	"MEMORY_INTEGER64",				// 4
	"MEMORY_FLOAT32",				// 5
	"MEMORY_FLOAT64",				// 6
	"MEMORY_ARRAY",					// 7
	"MEMORY_STRING",				// 8
	"MEMORY_APPLICATION"			// 9
};

// Method(s)
void* sce::Memory::New(void* pMem, size64 nSize, sce::MemoryTag tag)
{
#if defined(SCE_PLATFORM_DEBUG)
	std::wcout << __FUNCTIONW__ << std::endl;
#endif	

	PointerBlock pi = {
		pMem,
		sizeof(pMem),
		nSize,
		tag,
	};

	m_PointerInfo[m_nPointerCount] = pi;

	m_TotalPointerMemory += m_PointerInfo[m_nPointerCount].nPointerSize;
	m_TotalAllocatedMemory += m_PointerInfo[m_nPointerCount].nAllocationSize;
	m_TaggedAllocations[tag] += m_PointerInfo[m_nPointerCount].nPointerSize;

#if defined(SCE_PLATFORM_DEBUG)
	std::cout << "Memory allocated to (" << m_PointerInfo[m_nPointerCount].pPointer << ") " << m_StringArray[tag] << ": " << m_PointerInfo[m_nPointerCount].nPointerSize << " bytes" << std::endl;
	std::cout << "Memory allocation size: " << m_PointerInfo[m_nPointerCount].nAllocationSize << " bytes" << std::endl;

	std::cout << "Total pointer memory used: " << m_TotalPointerMemory << " bytes" << std::endl;
	std::cout << "Total allocation memory used: " << m_TotalAllocatedMemory << " bytes" << std::endl;
#endif

	m_nPointerCount++;
	return pMem;
}

void sce::Memory::Delete (void* pMem, size64 nSize, sce::MemoryTag tag)
{
#if defined(SCE_PLATFORM_DEBUG)
	std::wcout << __FUNCTIONW__ << std::endl;
#endif

	if (!pMem)
	{
		return;
	}

	for (int i = (m_nPointerCount - 1); i >= 0; i--)
	{
		if (m_PointerInfo[i].pPointer == pMem)
		{
			free(pMem);
			pMem = nullptr;			
						
			m_TotalPointerMemory -= m_PointerInfo[i].nPointerSize;
			m_TotalAllocatedMemory -= m_PointerInfo[i].nAllocationSize;

#if defined(SCE_PLATFORM_DEBUG)
			std::cout << "Memory allocation free from (" << m_PointerInfo[i].pPointer << ") " << m_StringArray[tag] << ": " << m_PointerInfo[i].nPointerSize << " bytes" << std::endl;
			std::cout << "Memory allocation size freed: " << m_PointerInfo[i].nAllocationSize << " bytes" << std::endl;

			std::cout << "Total pointer memory used: " << m_TotalPointerMemory << " bytes" << std::endl;
			std::cout << "Total allocation memory used: " << m_TotalAllocatedMemory << " bytes" << std::endl;
#endif
			m_PointerInfo[i].pPointer = nullptr;
			m_PointerInfo[i] = {};

			break;
		}
	}
}

void sce::Memory::DeallocateAll()
{
#if defined(SCE_PLATFORM_DEBUG)
	std::wcout << __FUNCTIONW__ << std::endl;
#endif

	for (int i = (m_nPointerCount - 1); i >= 0 ; i--)
	{
		if(m_PointerInfo[i].pPointer != nullptr)
			delete m_PointerInfo[i].pPointer;
	}
}

size64 sce::Memory::GetTotalPointerMemory()
{
	return m_TotalPointerMemory;
}

size64 sce::Memory::GetTotalAllocatedMemory()
{
	return m_TotalAllocatedMemory;
}