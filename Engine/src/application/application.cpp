#include "pch.h"
#include "application.h"

sce::Application::Application() : m_Str((c8*)calloc(1, sizeof(c8)))
{
#if defined(SCE_PLATFORM_DEBUG)
	std::wcout << __FUNCTIONW__ << std::endl;
#endif

}

sce::Application::~Application()
{
#if defined(SCE_PLATFORM_DEBUG)
	std::wcout << __FUNCTIONW__ << std::endl;
#endif

	sce::Memory::GetInstance()->DeallocateAll();
	
#if defined(SCE_PLATFORM_DEBUG)
	//std::wcout << __FUNCTIONW__ << std::endl;
	std::cout << "Total pointer memory: " << sce::Memory::GetInstance()->GetTotalPointerMemory() << " bytes" << std::endl;
	std::cout << "Total allocated memory: " << sce::Memory::GetInstance()->GetTotalAllocatedMemory() << " bytes" << std::endl;
#endif	
}

int sce::Application::Run()
{
#if defined(SCE_PLATFORM_DEBUG)
	std::wcout << __FUNCTIONW__ << std::endl;
#endif

	int* i = new int;
	int* j = new int[10];
	//delete i; 
	//delete j;

	int* k = new int(sce::MemoryTag::MEMORY_APPLICATION);

	return 0;
}
