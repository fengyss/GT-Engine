#include "gtpch.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "Log.h"

namespace GT {

	Ref<spdlog::logger> Log::s_CoreLogger;
	Ref<spdlog::logger> Log::s_ClientLogger;

	void Log::ShutDown()
	{
		GT_CORE_WARN("LOG DECONSTRUCTED!");

		s_CoreLogger.reset();
		s_ClientLogger.reset();
		s_CoreLogger = nullptr;
		s_ClientLogger = nullptr;

	}

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_CoreLogger = spdlog::stdout_color_mt("GT");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);

		GT_CORE_WARN("INITIALIZED LOG!");

	}

}