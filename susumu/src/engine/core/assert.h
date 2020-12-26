#pragma once

#include "engine/core/base.h"
#include "engine/core/log.h"

#include <filesystem>

#ifdef SU_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define SU_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { SU##type##ERROR(msg, __VA_ARGS__); SU_DEBUGBREAK(); } }
#define SU_INTERNAL_ASSERT_WITH_MSG(type, check, ...) SU_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define SU_INTERNAL_ASSERT_NO_MSG(type, check) SU_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", SU_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define SU_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define SU_INTERNAL_ASSERT_GET_MACRO(...) SU_EXPAND_MACRO( SU_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, SU_INTERNAL_ASSERT_WITH_MSG, SU_INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define SU_ASSERT(...) SU_EXPAND_MACRO( SU_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define SU_CORE_ASSERT(...) SU_EXPAND_MACRO( SU_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define SU_ASSERT(...)
#define SU_CORE_ASSERT(...)
#endif
