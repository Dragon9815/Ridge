#include "RidgePCH.h"
#include "Ridge/Core/RidgeEngine.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Ridge::RidgeEngine::Init();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		Ridge::RidgeEngine::Shutdown();
		break;
	}

	return TRUE;
}