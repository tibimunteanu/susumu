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
        //update scripts
        {
            m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
            {
                //TODO: move to Scene::OnScenePlay
                if (!nsc.Instance)
                {
                    nsc.Instance = nsc.InstantiateScript();
                    nsc.Instance->m_Entity = { entity, this };
                    nsc.Instance->OnCreate();
                }
                nsc.Instance->OnUpdate(dt);
            });
        }

        //render
        Camera* mainCamera = nullptr;
        glm::mat4* mainCameraTransform = nullptr;
        {
            auto view = m_Registry.view<TransformComponent, CameraComponent>();
            for (auto entity : view)
            {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
                if (camera.IsPrimary)
                {
                    mainCamera = &camera.Camera;
                    mainCameraTransform = &transform.Transform;
                    break;
                }
            }
        }

        if (mainCamera)
        {
            Renderer2D::BeginScene(mainCamera->GetProjection(), *mainCameraTransform);

            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group)
            {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::DrawQuad(transform, sprite.Color);
            }

            Renderer2D::EndScene();
        }
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        //resize non-IsFixedAspectRatio cameras
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.IsFixedAspectRatio)
            {
                cameraComponent.Camera.SetViewportSize(width, height);
            }
        }
    }
}
