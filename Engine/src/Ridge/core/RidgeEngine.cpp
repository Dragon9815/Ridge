#include "RidgePCH.h"
#include "RidgeEngine.h"

namespace Ridge {
	void RidgeEngine::Init()
	{
		Log::Init();
		RIDGE_CORE_INFO("Initializing Ridge Engine");
	}

	void RidgeEngine::Shutdown()
	{
		RIDGE_CORE_INFO("Shutting down Ridge Engine");
	}
}