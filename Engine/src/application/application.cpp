/*
* File name: Application.cpp
* File desc: Where the application runs.
* Author: Chloe Tunrer (chloeturner@tuta.io)
* Copyright: Copyright 2022 Chloe Tunrer
*/

/*
* ===========================================================
* Include(s)
* ===========================================================
*/
#include "pch.h"
#include "application.h"
#include "system/platform.h"

/* Application constructor
* Default
*/
sce::Application::Application()
{
#if defined(SCE_PLATFORM_DEBUG)
	printf("%s\n", __FUNCTION__);
#endif // !SCE_PLATFORM_DEBUG

	// Start up memory class
	Setup();
}

/* Application destructor
*/
sce::Application::~Application()
{
#if defined(SCE_PLATFORM_DEBUG)
	printf("%s\n", __FUNCTION__);
#endif // !SCE_PLATFORM_DEBUG

	// Shutdown memory class
	Shutdown();
}

const u32 max = 100;
i32* f1Mem[max];
u64* f2Mem[max];

v8 f1(const u32 c)
{
	for (u32 u = 0; u < c; u++)
	{
		f1Mem[u] = new(sce::MemoryTag::MEMORY_SINTEGER32) i32;
		//delete f1Mem[u];
	}
}

/* Runs the main application loop
*/
i32 sce::Application::Run()
{
#if defined(SCE_PLATFORM_DEBUG)
	printf("%s\n", __FUNCTION__);
#endif // !SCE_PLATFORM_DEBUG

	std::thread t1(f1, max);
	std::thread t2(f1, max);
	std::thread t3(f1, max);
	std::thread t4(f1, max);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	return 0;
}

/* Sets up the class for use
*/
v8 sce::Application::Setup()
{
	//sce::Platform::GetInstance().Setup();
}

/* Shuts down the class
*/
v8 sce::Application::Shutdown()
{
	sce::Platform::GetInstance().Shutdown();
}
