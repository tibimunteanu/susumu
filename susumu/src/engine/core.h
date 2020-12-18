#pragma once

#ifdef SU_PLATFORM_WINDOWS
#ifdef SU_DYNAMIC_LINK
    #ifdef SU_BUILD_DLL
        #define SU_API __declspec(dllexport)
    #else
        #define SU_API __declspec(dllimport)
    #endif
#else
    #define SU_API
#endif
#else
    #error susumu only runs on windows for now!
#endif

#ifdef SU_DEBUG
    #define SU_ENABLE_ASSERTS
#endif

#ifdef SU_ENABLE_ASSERTS
    #define SU_ASSERT(x, ...) { if (!(x)) { SU_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define SU_CORE_ASSERT(x, ...) { if(!(x)) { SU_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define SU_ASSERT(x, ...)
    #define SU_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define SU_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
