#pragma once

#include "engine/core/layer.h"
#include "engine/events/app_event.h"
#include "engine/events/key_event.h"
#include "engine/events/mouse_event.h"

namespace susumu
{
    class SU_API ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;

        void Begin();
        void End();
    private:
        float m_Time = 0.0f;
    };
}
