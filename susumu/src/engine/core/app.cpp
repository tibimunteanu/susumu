#include "supch.h"
#include "engine/core/app.h"
#include "engine/core/input.h"
#include "engine/core/log.h"
#include "engine/core/timestep.h"
#include "engine/renderer/renderer.h"
#include "engine/renderer/framebuffer.h"

//TEMP
#include <glad/glad.h>
#include <imgui.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <Windows.h>
#include <Commdlg.h>

namespace susumu
{
    App* App::s_Instance = nullptr;

    App::App(const std::string& name)
    {
        SU_CORE_ASSERT(!s_Instance, "App already exists");
        s_Instance = this;

        m_Window = Window::Create(WindowProps(name));
        m_Window->SetEventCallback(SU_BIND_EVENT_FN(App::OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        Renderer::Init();
        Renderer::Get().WaitAndRender();
    }

    App::~App()
    {
    }

    void App::Close()
    {
        m_Running = false;
    }

    void App::Run()
    {
        OnInit();
        while (m_Running)
        {
            if (!m_Minimized)
            {
                for (Layer* layer : m_LayerStack)
                {
                    layer->OnUpdate(m_Timestep);
                }

                App* app = this;
                SU_RENDER_1(app, { app->RenderImGui(); });

                Renderer::Get().WaitAndRender();
            }
            m_Window->OnUpdate();

            float time = (float)glfwGetTime();
            m_Timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;
        }
        OnShutdown();
    }

    void App::RenderImGui()
    {
        m_ImGuiLayer->Begin();
        {
            ImGui::Begin("Renderer");
            auto& caps = RendererAPI::GetCapabilities();
            ImGui::Text("Vendor: %s", caps.Vendor.c_str());
            ImGui::Text("Renderer: %s", caps.Renderer.c_str());
            ImGui::Text("Version: %s", caps.Version.c_str());
            ImGui::Text("Frame Time: %.2fms\n", m_Timestep.GetMilliseconds());
            ImGui::End();

            for (Layer* layer : m_LayerStack)
            {
                layer->OnImGuiRender();
            }
        }
        m_ImGuiLayer->End();
    }

    void App::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(SU_BIND_EVENT_FN(App::OnWindowClosed));
        dispatcher.Dispatch<WindowResizeEvent>(SU_BIND_EVENT_FN(App::OnWindowResize));

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        {
            if (e.Handled)
            {
                break;
            }
            (*it)->OnEvent(e);
        }
    }

    std::string App::OpenFile(const std::string& filter) const
    {
        OPENFILENAMEA ofn;       // common dialog box structure
        CHAR szFile[260] = { 0 };       // if using TCHAR macros

        // Initialize OPENFILENAME
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)m_Window->GetNativeWindow());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "All\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileNameA(&ofn) == TRUE)
        {
            return ofn.lpstrFile;
        }
        return std::string();
    }

    bool App::OnWindowClosed(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    bool App::OnWindowResize(WindowResizeEvent& e)
    {
        uint32_t width = e.GetWidth();
        uint32_t height = e.GetHeight();

        if (width == 0 || height == 0)
        {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;

        SU_RENDER_2(width, height, {
            glViewport(0, 0, width, height);
            });

        auto& fbs = FramebufferPool::GetGlobal()->GetAll();
        for (auto& fb : fbs)
        {
            fb->Resize(width, height);
        }

        return false;
    }

	float App::GetTime() const
	{
		return (float)glfwGetTime();
	}
}
