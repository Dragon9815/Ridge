#pragma once

#include <memory>
#include "Ridge/Common.h"

#include "spdlog/spdlog.h"

namespace Ridge {
	class RIDGE_API Log {
	public:
		inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return s_engineLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetAppLogger() { return s_appLogger; }

		static void Init();

	private:
		static std::shared_ptr<spdlog::logger> s_engineLogger;
		static std::shared_ptr<spdlog::logger> s_appLogger;
	};
};

// ------------------------------------- Engine Logging --------------------------------------------------------
#define RIDGE_CORE_TRACE(fmt, ...)	::Ridge::Log::GetEngineLogger()->trace(fmt, __VA_ARGS__);
#define RIDGE_CORE_INFO(fmt, ...)	::Ridge::Log::GetEngineLogger()->info(fmt, __VA_ARGS__);
#define RIDGE_CORE_WARN(fmt, ...)	::Ridge::Log::GetEngineLogger()->warn(fmt, __VA_ARGS__);
#define RIDGE_CORE_ERROR(fmt, ...)	::Ridge::Log::GetEngineLogger()->error(fmt, __VA_ARGS__);

// ------------------------------------- App Logging -----------------------------------------------------------
#define RIDGE_TRACE(fmt, ...)	::Ridge::Log::GetAppLogger()->trace(fmt, __VA_ARGS__);
#define RIDGE_INFO(fmt, ...)	::Ridge::Log::GetAppLogger()->info(fmt, __VA_ARGS__);
#define RIDGE_WARN(fmt, ...)	::Ridge::Log::GetAppLogger()->warn(fmt, __VA_ARGS__);
#define RIDGE_ERROR(fmt, ...)	::Ridge::Log::GetAppLogger()->error(fmt, __VA_ARGS__);

