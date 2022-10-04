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

const u32 max = 1;
u64* f1Mem[max];

v8 f1(const u32 c)
{
	for (u32 u = 0; u < c; u++)
	{
		//f1Mem[u] = new(sce::MemoryFlag::MEMORY_UINTEGER64) u64;
		////delete f1Mem[u];

		//if (u % 100 == 0)
		//{
		//	printf("%s\n", "working . . .");
		//}

		sce::Platform::GetInstance().SetTextColour(sce::ConsoleTextColour::Red);		
		sce::Platform::GetInstance().Print(sce::ConsoleStream::CONSOLE_STDERROR, "Hello world! (const) (ascii)\n");
		sce::Platform::GetInstance().SetTextColour(sce::ConsoleTextColour::Blue);
		sce::Platform::GetInstance().Print(sce::ConsoleStream::CONSOLE_STDOUT, L"Hello world! (const) (unicode)\n");

		/*c8* sz8 = const_cast<c8*>("Hello world! (ascii)\n");
		c16* sz16 = const_cast<c16*>(L"Hello world! (unicode)\n");

		sce::Platform::GetInstance().Print(sce::ConsoleStream::CONSOLE_STDERROR, sz8);
		sce::Platform::GetInstance().Print(sce::ConsoleStream::CONSOLE_STDOUT, sz16);*/

		c8 szB8[256]	= "Hello world (ascii)\n";
		c16 szB16[256]	= L"Hello world (unicode)\n";

		//sce::Platform::GetInstance().Read(szB8, 256);
		//sce::Platform::GetInstance().Read(szB16, 256);

		sce::Platform::GetInstance().SetTextColour(sce::ConsoleTextColour::LightGreen);
		sce::Platform::GetInstance().Print(sce::ConsoleStream::CONSOLE_STDERROR, szB8);
		sce::Platform::GetInstance().SetTextColour(sce::ConsoleTextColour::LightMagenta);
		sce::Platform::GetInstance().Print(sce::ConsoleStream::CONSOLE_STDOUT, szB16);

		sce::Platform::GetInstance().ResetTextColour();
	}
}

/* Runs the main application loop
*/
i32 sce::Application::Run()
{
//#if defined(SCE_PLATFORM_DEBUG)
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG
// 
	std::thread t1(f1, max);
	/*std::thread t2(f1, max);
	std::thread t3(f1, max);
	std::thread t4(f1, max);

	std::thread t5(f1, max);
	std::thread t6(f1, max);
	std::thread t7(f1, max);
	std::thread t8(f1, max);*/

	t1.join();
	/*t2.join();
	t3.join();
	t4.join();

	t5.join();
	t6.join();
	t7.join();
	t8.join();*/

	return 0;
}

/* Sets up the class for use
*/
v8 sce::Application::Setup()
{
//#if defined(SCE_PLATFORM_DEBUG)
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

}

/* Shuts down the class
*/
v8 sce::Application::Shutdown()
{
//#if defined(SCE_PLATFORM_DEBUG)
//	printf("%s\n", __FUNCTION__);
//#endif // !SCE_PLATFORM_DEBUG

	//wprintf(L"%s\n", sce::Platform::GetInstance().StatsToString());
	sce::Platform::GetInstance().Shutdown();
	//wprintf(L"%s\n", sce::Platform::GetInstance().StatsToString());
}
