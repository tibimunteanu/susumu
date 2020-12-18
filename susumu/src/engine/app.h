#pragma once

#include "core.h"
#include "window.h"
#include "engine/layer_stack.h"
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

    private:
        bool m_Running = true;
        std::unique_ptr<Window> m_Window;
        LayerStack m_LayerStack;
        ImGuiLayer* m_ImGuiLayer;
    private:
        static App* s_Instance;
    };

    //to be defined in client
    App* CreateApp();

}
