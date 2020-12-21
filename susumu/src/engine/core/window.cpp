#include "supch.h"
#include "engine/core/window.h"

#ifdef SU_PLATFORM_WINDOWS
#include "platform/windows/windows_window.h"
#endif

namespace susumu
{
    Scope<Window> Window::Create(const WindowProps& props)
    {
    #ifdef SU_PLATFORM_WINDOWS
        return CreateScope<WindowsWindow>(props);
    #else
        SU_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
    #endif
    }
}