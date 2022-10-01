/*
* File name: Application.cpp
* File desc: Where the application runs.
* Author: Chloe Tunrer (chloeturner@tuta.io)
* Copyright: Copyright 2022 Chloe Tunrer
*/

#include "pch.h"
#include "application.h"
#include "system/platform.h"

sce::Application::Application()
{
#if defined(SCE_PLATFORM_DEBUG)
	printf("%s\n", __FUNCTION__);
#endif // !SCE_PLATFORM_DEBUG

	// Start up memory class
}

sce::Application::~Application()
{
#if defined(SCE_PLATFORM_DEBUG)
	printf("%s\n", __FUNCTION__);
#endif // !SCE_PLATFORM_DEBUG

	// Shutdown memory class
	sce::plfm::PlatformAllocator::Shutdown();
}

const u32 max = 1;
i32* p1Mem[max];

v8 p1(const u32 c)
{
	for (u32 u = 0; u < c; u++)
	{
		p1Mem[u] = new i32;
		delete p1Mem[u];
		//printf("%s %s\n", __FUNCTION__, "pass");
	}
}

v8 f1(const u32 c)
{
	p1(c);
	//printf("%s %s\n", __FUNCTION__, "complete");
}

v8 f2(const u32 c)
{
	p1(c);
	//printf("%s %s\n", __FUNCTION__, "complete");
}

i32 sce::Application::Run()
{
#if defined(SCE_PLATFORM_DEBUG)
	printf("%s\n", __FUNCTION__);
#endif // !SCE_PLATFORM_DEBUG

	//std::thread t1(f1, max);
	//std::thread t2(f2, max);

	f32* float32 = new(sce::MemoryTag::MEMORY_FLOAT32) f32;
	c16* char16 = new(sce::MemoryTag::MEMORY_CHAR16) c16;

	//t1.join();
	//t2.join();
	
	return 0;
}
