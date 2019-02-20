#include "RidgePCH.h"
#include "RidgeEngine.h"

#include "Ridge/Input/Input.h"

#define RIDGE_BUILD_ID "v0.0.1a"

namespace Ridge {
	void RidgeEngine::Init()
	{
		Log::Init();
		RIDGE_CORE_TRACE("Initializing Ridge Engine {}", RIDGE_BUILD_ID);
		if (!Input::Initalize()) {
			RIDGE_CORE_ERROR("Failed to intialize input");
		}
	}

	void RidgeEngine::Shutdown()
	{
		RIDGE_CORE_TRACE("Shutting down Ridge Engine");
	}
}