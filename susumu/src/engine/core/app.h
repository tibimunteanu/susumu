#pragma once

#include "engine/core/base.h"
#include "engine/core/window.h"
#include "engine/core/layer_stack.h"
#include "engine/events/event.h"
#include "engine/events/app_event.h"
#include "engine/imgui/imgui_layer.h"

int main(int argc, char** argv);

namespace susumu
{
    class App
    {
    public:
        App();
        virtual ~App();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }
        void PopLayer(Layer* layer) { m_LayerStack.PopLayer(layer); }
        void PushOverlay(Layer* overlay) { m_LayerStack.PushLayer(overlay); }
        void PopOverlay(Layer* overlay) { m_LayerStack.PopLayer(overlay); }

        Window& GetWindow() { return *m_Window; }
        float GetLastTimeStep() { return m_LastTimeStep; }
        static App& Get() { return *s_Instance; }
    private:
        void Run();
        bool OnWindowClosed(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

    private:
        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        LayerStack m_LayerStack;
        bool m_Running = true;
        bool m_Minimized = false;
        float m_LastFrameTime = 0.0f;
        Timestep m_LastTimeStep;
    private:
        static App* s_Instance;
        friend int ::main(int argc, char** argv);
    };

    //to be defined in client
    App* CreateApp();
}
