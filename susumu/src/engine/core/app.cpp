#include "supch.h"
#include "engine/core/app.h"
#include "engine/core/input.h"
#include "engine/core/log.h"
#include "engine/core/timestep.h"
#include "engine/renderer/renderer.h"

//TEMP
#include <GLFW/glfw3.h>

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
    }

    App::~App()
    {
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

    void App::Close()
    {
        m_Running = false;
    }

    void App::Run()
    {
        while (m_Running)
        {
            float time = (float)glfwGetTime(); //TODO: Platform::GetTime
            m_LastTimeStep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            if (!m_Minimized)
            {
                for (Layer* layer : m_LayerStack)
                {
                    layer->OnUpdate(m_LastTimeStep);
                }
            }

            App* app = this;
            SU_RENDER_1(app, { app->RenderImGui(); });

            Renderer::Get().WaitAndRender();

            m_Window->OnUpdate();
        }
    }

    void App::RenderImGui()
    {
        m_ImGuiLayer->Begin();
        {
            for (Layer* layer : m_LayerStack)
            {
                layer->OnImGuiRender();
            }
        }
        m_ImGuiLayer->End();
    }

    bool App::OnWindowClosed(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    bool App::OnWindowResize(WindowResizeEvent& e)
    {
        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;

        return false;
    }
}
