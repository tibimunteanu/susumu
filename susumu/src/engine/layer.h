#pragma once

#include "engine/core.h"
#include "engine/events/event.h"
#include "engine/core/timestep.h"

namespace susumu {

    class SU_API Layer
    {
    public:
        Layer(const std::string& debugName = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep dt) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

        inline const std::string& GetName() const { return m_DebugName; }
    protected:
        std::string m_DebugName;
    };

}
