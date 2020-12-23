#pragma once

#include "engine/core/timestep.h"

#include <entt.hpp>

namespace susumu
{
    class Scene
    {
    public:
        Scene();
        ~Scene();

        entt::entity CreateEntity();

        // TEMP
        entt::registry& Reg() { return m_Registry; }

        void OnUpdate(Timestep dt);
    private:
        entt::registry m_Registry;
    };
}

