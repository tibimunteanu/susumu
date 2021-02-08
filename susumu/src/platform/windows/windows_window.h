#pragma once

#include "engine/core/window.h"

#include <GLFW/glfw3.h>

namespace susumu
{
    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void OnUpdate() override;

        uint32_t GetWidth() const override { return m_Data.Width; }
        uint32_t GetHeight() const override { return m_Data.Height; }
        virtual std::pair<float, float> WindowsWindow::GetWindowPos() const override;

        void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;
        virtual void* GetNativeWindow() const { return m_Window; }
    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();
    private:
        GLFWwindow* m_Window;
		GLFWcursor* m_ImGuiMouseCursors[9] = { 0 };

        struct WindowData
        {
            std::string Title;
            uint32_t Width;
            uint32_t Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };
}
