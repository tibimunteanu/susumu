#include "supch.h"
#include "windows_input.h"
#include "engine/core/app.h"

#include <GLFW/glfw3.h>

namespace susumu {

    Scope<Input> Input::s_Instance = CreateScope<WindowsInput>();

    bool WindowsInput::IsKeyPressedImpl(int keycode)
    {
        auto window = static_cast<GLFWwindow*>(App::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool WindowsInput::IsMouseButtonPressedImpl(int button)
    {
        auto window = static_cast<GLFWwindow*>(App::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> WindowsInput::GetMousePositionImpl()
    {
        auto window = static_cast<GLFWwindow*>(App::Get().GetWindow().GetNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        return { (float)xPos, (float)yPos };
    }

    float WindowsInput::GetMouseXImpl()
    {
        auto[x, y] = GetMousePositionImpl();
        return x;
    }

    float WindowsInput::GetMouseYImpl()
    {
        auto[x, y] = GetMousePositionImpl();
        return y;
    }

}
