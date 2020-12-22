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

    App::App()
    {
        SU_PROFILE_FUNCTION();

        SU_CORE_ASSERT(!s_Instance, "App already exists");
        s_Instance = this;

        m_Window = Window::Create();
        m_Window->SetEventCallback(SU_BIND_EVENT_FN(App::OnEvent));

        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    App::~App()
    {
        Renderer::Shutdown();
    }

    void App::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(SU_BIND_EVENT_FN(App::OnWindowClosed));
        dispatcher.Dispatch<WindowResizeEvent>(SU_BIND_EVENT_FN(App::OnWindowResize));

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        {
            (*it)->OnEvent(e);
            if (e.Handled)
            {
                break;
            }
        }
    }

    void App::Run()
    {
        SU_PROFILE_FUNCTION();

        while (m_Running)
        {
            SU_PROFILE_SCOPE("RunLoop");

            float time = (float)glfwGetTime(); //TODO: Platform::GetTime
            m_LastTimeStep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            if (!m_Minimized)
            {
                {
                    SU_PROFILE_SCOPE("LayerStack OnUpdate");
                    for (Layer* layer : m_LayerStack)
                    {
                        layer->OnUpdate(m_LastTimeStep);
                    }
                }
            }

            m_ImGuiLayer->Begin();
            {
                SU_PROFILE_SCOPE("LayerStack OnImGuiRender");
                for (Layer* layer : m_LayerStack)
                {
                    layer->OnImGuiRender();
                }
            }
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    bool App::OnWindowClosed(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    bool App::OnWindowResize(WindowResizeEvent& e)
    {
        SU_PROFILE_FUNCTION();

        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

        return false;
    }
}
