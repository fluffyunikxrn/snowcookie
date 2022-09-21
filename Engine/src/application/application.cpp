#include "sce_pch.h"
#include "application.h"

sce::Application::Application()
{
	std::wcout << __FUNCTIONW__ << std::endl;
}

sce::Application::~Application()
{
	std::wcout << __FUNCTIONW__ << std::endl;
}

int sce::Application::Run()
{
	std::wcout << __FUNCTIONW__ << std::endl;

	return 0;
}
