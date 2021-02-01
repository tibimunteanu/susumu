#include "supch.h"
#include "platform/windows/windows_window.h"
#include "engine/core/input.h"
#include "engine/events/app_event.h"
#include "engine/events/key_event.h"
#include "engine/events/mouse_event.h"
#include "engine/renderer/renderer.h"

#include <glad/glad.h>
#include <imgui.h>

namespace susumu
{
    static uint8_t s_GLFWWindowCount = 0;

    static void GLFWErrorCallback(int error, const char* description)
    {
        SU_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        SU_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        if (s_GLFWWindowCount == 0)
        {
            {
                int glfwSuccess = glfwInit();
                SU_CORE_ASSERT(glfwSuccess, "Could not initialize GLFW!");
                glfwSetErrorCallback(GLFWErrorCallback);
            }
        }

        {
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
            m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
            s_GLFWWindowCount++;
        }

        glfwMakeContextCurrent(m_Window);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        SU_CORE_ASSERT(status, "Failed to initialize Glad!");

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        //set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int modes)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, 1);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            KeyTypedEvent event(keycode);
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int modes)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xPos, (float)yPos);
            data.EventCallback(event);
        });

        m_ImGuiMouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        m_ImGuiMouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
        m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);   // FIXME: GLFW doesn't have this.
        m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
        m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
        m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
        m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
        m_ImGuiMouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    }

    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);

        s_GLFWWindowCount--;
        if (s_GLFWWindowCount == 0)
        {
            glfwTerminate();
        }
    }

    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);

        ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
        glfwSetCursor(m_Window, m_ImGuiMouseCursors[imgui_cursor] ? m_ImGuiMouseCursors[imgui_cursor] : m_ImGuiMouseCursors[ImGuiMouseCursor_Arrow]);
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        glfwSwapInterval(enabled ? 1 : 0);
        m_Data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }
}
