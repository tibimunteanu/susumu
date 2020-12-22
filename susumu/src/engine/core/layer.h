#pragma once

#include "engine/core/core.h"
#include "engine/core/timestep.h"
#include "engine/events/event.h"

namespace susumu
{
    class Layer
    {
    public:
        Layer(const std::string& debugName = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep dt) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

        const std::string& GetName() const { return m_DebugName; }
    protected:
        std::string m_DebugName;
    };
}
