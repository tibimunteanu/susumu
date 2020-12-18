#include "supch.h"
#include "app.h"
#include "input.h"
#include "core/log.h"

namespace susumu {

    App* App::s_Instance = nullptr;

    App::App()
    {
        SU_CORE_ASSERT(!s_Instance, "App already exists");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(SU_BIND_EVENT_FN(App::OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    void App::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void App::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
    }

    void App::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(SU_BIND_EVENT_FN(App::OnWindowClosed));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
            {
                break;
            }
        }
    }

    void App::Run()
    {
        while (m_Running)
        {
            for (Layer* layer : m_LayerStack)
            {
                layer->OnUpdate();
            }

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
            {
                layer->OnImGuiRender();
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

}
