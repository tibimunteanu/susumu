#pragma once

#include "engine/core/layer.h"
#include "engine/events/app_event.h"
#include "engine/events/key_event.h"
#include "engine/events/mouse_event.h"

namespace susumu
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& e) override;

        void Begin();
        void End();

        void SetBlockEvents(bool block) { m_BlockEvents = block; }
        void SetDarkThemeColors();
    private:
        bool m_BlockEvents = true;
        float m_Time = 0.0f;
    };
}
