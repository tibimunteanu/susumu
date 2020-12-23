#pragma once

#include "engine/scene/scene_camera.h"

#include<glm/glm.hpp>

namespace susumu
{
    struct TagComponent
    {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag) 
            : Tag(tag) {}
    };

    struct TransformComponent
    {
        glm::mat4 Transform{ 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& transform) 
            : Transform(transform) {}
        operator glm::mat4& () { return Transform; }
        operator const glm::mat4& () const { return Transform; }
    };

    struct SpriteRendererComponent
    {
        glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color) 
            : Color(color) {}
    };

    struct CameraComponent
    {
        susumu::SceneCamera Camera;
        bool IsPrimary = true;
        bool IsFixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };
}

