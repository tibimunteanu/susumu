#include "supch.h"
#include "engine/scene/entity.h"

namespace susumu
{
    Entity::Entity(entt::entity handle, Scene* scene)
        : m_EntityHandle(handle), m_Scene(scene)
    {

    }
}
