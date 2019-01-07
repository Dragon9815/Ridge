#pragma once

#include "Ridge/Application.h"
#include "Ridge/core/RidgeEngine.h"

#ifdef RIDGE_PLATFORM_WINDOWS
extern Ridge::Application* CreateApplication();

int main(int argc, char** argv)
{
	Ridge::RidgeEngine::Init();

	auto app = CreateApplication();
	app->Start();
	delete app;

	Ridge::RidgeEngine::Shutdown();
}
#endif