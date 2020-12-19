#pragma once

#include "core.h"
#include "window.h"
#include "layer_stack.h"
#include "engine/events/event.h"
#include "engine/events/app_event.h"
#include "engine/imgui/imgui_layer.h"

namespace susumu {

    class SU_API App
    {
    public:
        App();
        virtual ~App() = default;

        void Run();
        void OnEvent(Event& e);
        void PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }
        void PushOverlay(Layer* overlay) { m_LayerStack.PushOverlay(overlay); }

        inline Window& GetWindow() { return *m_Window; }
        inline static App& Get() { return *s_Instance; }
    private:
        bool OnWindowClosed(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

    private:
        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        LayerStack m_LayerStack;
        bool m_Running = true;
        bool m_Minimized = false;
        float m_LastFrameTime = 0.0f;
    private:
        static App* s_Instance;
    };

    //to be defined in client
    App* CreateApp();

}
