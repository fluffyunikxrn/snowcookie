#pragma once

#include "defines.h"
#include "using.h"
#include "patterns/isingleton.h"

#if not defined(_NEW_)
#include <new>
#endif

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
void* __CRTDECL operator new(size_t nSize);

_NODISCARD _Ret_maybenull_ _Success_(return != NULL) _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
void* __CRTDECL operator new(size_t nSize, ::std::nothrow_t const&) noexcept;

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
void* __CRTDECL operator new[](size_t nSize);

_NODISCARD _Ret_maybenull_ _Success_(return != NULL) _Post_writable_byte_size_(nSize) _VCRT_ALLOCATOR
void* __CRTDECL operator new[](size_t nSize, ::std::nothrow_t const&) noexcept;

void __CRTDECL operator delete(void* _Block) noexcept;

void __CRTDECL operator delete(void* _Block, ::std::nothrow_t const&) noexcept;

void __CRTDECL operator delete[](void* _Block) noexcept;

void __CRTDECL operator delete[](void* _Block, ::std::nothrow_t const&) noexcept;

void __CRTDECL operator delete(void* _Block, size_t _Size) noexcept;

void __CRTDECL operator delete[](void* _Block, size_t _Size) noexcept;

namespace sce
{
	// 263953113
	const u64 g_nMaxPointers = 20000000;

	typedef enum MEMORYTAG
	{
		MEMORY_UNKNOWN				= 0,
		MEMORY_INTEGER8				= 1,
		MEMORY_INTEGER16			= 2,
		MEMORY_INTEGER32			= 3,
		MEMORY_INTEGER64			= 4,
		MEMORY_FLOAT32				= 5,
		MEMORY_FLOAT64				= 6,
		MEMORY_ARRAY				= 7,
		MEMORY_STRING				= 8,
		MEMORY_APPLICATION			= 9,

		MEMORY_MAXMEMORYTAGS,
	} MemoryTag;

	typedef struct POINTERBLOCK
	{
	public:
		void* pPointer;
		size64 nPointerSize;
		size64 nAllocationSize;
		MemoryTag Tag;
	} PointerBlock;

	class PointerInfo
	{
		size64 TotalPointerMemory;
		size64 TotalAllocatedMemory;
		size64 TaggedAllocations[MemoryTag::MEMORY_MAXMEMORYTAGS];
		PointerBlock PointerInfo[g_nMaxPointers];
		u64 nPointerCount;
		const char* const StringArray[sce::MemoryTag::MEMORY_MAXMEMORYTAGS] = 
		{
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
	};

	class SCEAPI Memory : public ISingleton<Memory>
	{
	private:
		static size64 m_TotalPointerMemory;
		static size64 m_TotalAllocatedMemory;
		static size64 m_TaggedAllocations[MemoryTag::MEMORY_MAXMEMORYTAGS];
		static PointerBlock m_PointerInfo[g_nMaxPointers];
		static u64 m_nPointerCount;
		static const char* const m_StringArray[sce::MemoryTag::MEMORY_MAXMEMORYTAGS];

	public:
		// Constructor(s)
		Memory();
		~Memory() override;

		// Method(s)
		void* New(void* pMem, size64 nSize, MemoryTag sTag);
		void Delete(void* pMem, size64 nSize, MemoryTag sTag);

		void DeallocateAll();

		static size64 GetTotalPointerMemory();
		static size64 GetTotalAllocatedMemory();
	};
}
