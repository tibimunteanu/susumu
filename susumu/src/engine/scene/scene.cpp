#include "supch.h"
#include "scene.h"

#include "engine/scene/components.h"
#include "engine/scene/entity.h"
#include "engine/renderer/renderer_2d.h"

namespace susumu
{
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Untagged" : name;
        return entity;
    }

    void Scene::OnUpdate(Timestep dt)
    {
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group)
        {
            auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::DrawQuad(transform, sprite.Color);
        }
    }
}
