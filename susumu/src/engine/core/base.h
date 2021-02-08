#pragma once

#include <memory>

#include "engine/core/platform_detection.h"


#ifdef SU_DEBUG
	#if defined(SU_PLATFORM_WINDOWS)
		#define SU_DEBUGBREAK() __debugbreak()
	#elif defined(SU_PLATFORM_LINUX)
		#include <signal.h>
		#define SU_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define SU_ENABLE_ASSERTS
#else
	#define SU_DEBUGBREAK()
#endif

#define SU_EXPAND_MACRO(x) x
#define SU_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define SU_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace susumu 
{
    template<typename T>
    using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

    template<typename T>
    using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#include "engine/core/log.h"
#include "engine/core/assert.h"

