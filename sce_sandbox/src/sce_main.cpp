#include "debuger/sce_log.h"
#include "app/sce_app.h"
//#include "utilities/sce_strings.h"
#include <iostream>

int main(int argc, char** argv)
{	
	/*sce::Application* app = new sce::Application();
	app->PrintText("Hello, Harry");
	delete app;*/

	sce::LogW* l0 = new sce::LogW(L"console", sce::LogFlags::Stderr | sce::LogFlags::Prefix | sce::LogFlags::TimeStamp | sce::LogFlags::FileLine | sce::LogFlags::Function | sce::LogFlags::Compact);

	l0->Print(L"Hello world");\

	if (l0)
		delete l0;

	sce::LogW* l1 = new sce::LogW(L"file", L"test_w.log", sce::LogFlags::Stderr | sce::LogFlags::Outfile | sce::LogFlags::Prefix | sce::LogFlags::TimeStamp | sce::LogFlags::FileLine | sce::LogFlags::Function | sce::LogFlags::Compact);

	l1->Print(L"Hello world");

	if (l1)
		delete l1;

	sce::LogA* l3 = new sce::LogA("console", sce::LogFlags::Stderr | sce::LogFlags::Prefix | sce::LogFlags::TimeStamp | sce::LogFlags::FileLine | sce::LogFlags::Function | sce::LogFlags::Compact);

	l3->Print("Hello world");

	if (l3)
		delete l3;

	sce::LogA* l4 = new sce::LogA("file", "test_a.log", sce::LogFlags::Stderr | sce::LogFlags::Outfile | sce::LogFlags::Prefix | sce::LogFlags::TimeStamp | sce::LogFlags::FileLine | sce::LogFlags::Function | sce::LogFlags::Compact);

	l4->Print("Hello world");

	if (l4)
		delete l4;

	sce::Log* l5 = new sce::Log(L"file", L"test_l.log", sce::LogFlags::Stderr | sce::LogFlags::Outfile | sce::LogFlags::Prefix | sce::LogFlags::TimeStamp | sce::LogFlags::FileLine | sce::LogFlags::Function | sce::LogFlags::Compact);

	l5->Print(L"Hello world");

	if (l5)
		delete l5;
}
