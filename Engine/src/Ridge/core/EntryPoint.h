#pragma once

#include "Ridge/Core/Application.h"
#include "Ridge/Core/RidgeEngine.h"

#ifdef RIDGE_PLATFORM_WINDOWS
extern Ridge::Application* CreateApplication();

int main(int argc, char** argv)
{
	auto app = CreateApplication();
	app->Run();
	delete app;
}
#endif