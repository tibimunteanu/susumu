#include "supch.h"
#include "engine/core/input.h"

#ifdef SU_PLATFORM_WINDOWS
#include "platform/windows/windows_input.h"
#endif

namespace susumu
{
    Scope<Input> Input::s_Instance = Input::Create();

    Scope<Input> Input::Create()
    {
    #ifdef SU_PLATFORM_WINDOWS
        return CreateScope<WindowsInput>();
    #else
        SU_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
    #endif
    }
}
