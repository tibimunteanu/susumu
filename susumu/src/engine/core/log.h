#pragma once

#include "engine/core/base.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace susumu
{
    class Log
    {
    public:
        static void Init();

        static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static Ref<spdlog::logger>& GetAppLogger() { return s_AppLogger; }
    private:
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_AppLogger;
    };
}

//core log macros
#define SU_CORE_TRACE(...) ::susumu::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SU_CORE_INFO(...) ::susumu::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SU_CORE_WARN(...) ::susumu::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SU_CORE_ERROR(...) ::susumu::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SU_CORE_CRITICAL(...) ::susumu::Log::GetCoreLogger()->critical(__VA_ARGS__)

//client log macros
#define SU_TRACE(...) ::susumu::Log::GetAppLogger()->trace(__VA_ARGS__)
#define SU_INFO(...) ::susumu::Log::GetAppLogger()->info(__VA_ARGS__)
#define SU_WARN(...) ::susumu::Log::GetAppLogger()->warn(__VA_ARGS__)
#define SU_ERROR(...) ::susumu::Log::GetAppLogger()->error(__VA_ARGS__)
#define SU_CRITICAL(...) ::susumu::Log::GetAppLogger()->critical(__VA_ARGS__)
