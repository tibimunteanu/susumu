#pragma once

#include "core.h"
#include "window.h"
#include "core/layer_stack.h"
#include "core/events/event.h"
#include "core/events/app_event.h"
#include "core/imgui/imgui_layer.h"

namespace susumu {

    class SU_API App
    {
    public:
        App();
        virtual ~App();

        void Run();
        void OnEvent(Event& e);
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        inline Window& GetWindow() { return *m_Window; }
        inline static App& Get() { return *s_Instance; }
    private:
        bool OnWindowClosed(WindowCloseEvent& e);

    private:
        bool m_Running = true;
        std::unique_ptr<Window> m_Window;
        LayerStack m_LayerStack;
        ImGuiLayer* m_ImGuiLayer;

        unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
    private:
        static App* s_Instance;
    };

    //to be defined in client
    App* CreateApp();

}
