#include "RidgePCH.h"
#include "Log.h"

#include "spdlog\sinks\stdout_color_sinks.h"

#include <fcntl.h>


namespace Ridge {
	std::shared_ptr<spdlog::logger> Log::s_engineLogger;
	std::shared_ptr<spdlog::logger> Log::s_appLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_engineLogger = spdlog::stdout_color_mt("RIDGE");
		s_engineLogger->set_level(spdlog::level::trace);

		s_appLogger = spdlog::stdout_color_mt("APP");
		s_appLogger->set_level(spdlog::level::trace);
	}
}
