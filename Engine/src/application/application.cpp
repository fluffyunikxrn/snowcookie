/*
* File name: Application.cpp
* File desc: Where the application runs.
* Author: Chloe Tunrer (chloeturner@tuta.io)
* Copyright: Copyright 2022 Chloe Tunrer
*/

#include "pch.h"
#include "application.h"

sce::Application::Application() : m_Str((c8*)calloc(1, sizeof(c8)))
{
#if defined(SCE_PLATFORM_DEBUG)
	printf("%s\n", __FUNCTION__);
#endif // !SCE_PLATFORM_DEBUG

}

sce::Application::~Application()
{
#if defined(SCE_PLATFORM_DEBUG)
	printf("%s\n", __FUNCTION__);
#endif // !SCE_PLATFORM_DEBUG

	// Clean allocation(s)
	sce::Memory::GetInstance()->ShutdownMemory();
}

i32 sce::Application::Run()
{
#if defined(SCE_PLATFORM_DEBUG)
	printf("%s\n", __FUNCTION__);
#endif // !SCE_PLATFORM_DEBUG

 	i16* i1 = new(sce::MemoryTag::MEMORY_INTEGER16) i16;
	i32* i2 = new(sce::MemoryTag::MEMORY_INTEGER32) i32;
	i64* i3 = new(sce::MemoryTag::MEMORY_INTEGER64) i64;
	delete i1;
	delete i2;
	delete i3;

	return 0;
}
