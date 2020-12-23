#pragma once

#include "engine/core/timestep.h"

#include <entt.hpp>

namespace susumu
{
    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());

        void OnUpdate(Timestep dt);
    private:
        entt::registry m_Registry;

        friend class Entity;
    };
}

