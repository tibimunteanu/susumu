#pragma once

#include <memory>

#include "core.h"
#include "spdlog/spdlog.h"

namespace susumu {

	class SU_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

//core log macros
#define SU_CORE_FATAL(...) ::susumu::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define SU_CORE_ERROR(...) ::susumu::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SU_CORE_WARN(...) ::susumu::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SU_CORE_INFO(...) ::susumu::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SU_CORE_TRACE(...) ::susumu::Log::GetCoreLogger()->trace(__VA_ARGS__)

//client log macros
#define SU_FATAL(...) ::susumu::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define SU_ERROR(...) ::susumu::Log::GetClientLogger()->error(__VA_ARGS__)
#define SU_WARN(...) ::susumu::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SU_INFO(...) ::susumu::Log::GetClientLogger()->info(__VA_ARGS__)
#define SU_TRACE(...) ::susumu::Log::GetClientLogger()->trace(__VA_ARGS__)
