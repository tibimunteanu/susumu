#pragma once

#include "core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace susumu {

    class SU_API Log
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetAppLogger() { return s_AppLogger; }
    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_AppLogger;
    };

}

//core log macros
#define SU_CORE_FATAL(...) ::susumu::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define SU_CORE_ERROR(...) ::susumu::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SU_CORE_WARN(...) ::susumu::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SU_CORE_INFO(...) ::susumu::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SU_CORE_TRACE(...) ::susumu::Log::GetCoreLogger()->trace(__VA_ARGS__)

//client log macros
#define SU_FATAL(...) ::susumu::Log::GetAppLogger()->critical(__VA_ARGS__)
#define SU_ERROR(...) ::susumu::Log::GetAppLogger()->error(__VA_ARGS__)
#define SU_WARN(...) ::susumu::Log::GetAppLogger()->warn(__VA_ARGS__)
#define SU_INFO(...) ::susumu::Log::GetAppLogger()->info(__VA_ARGS__)
#define SU_TRACE(...) ::susumu::Log::GetAppLogger()->trace(__VA_ARGS__)
