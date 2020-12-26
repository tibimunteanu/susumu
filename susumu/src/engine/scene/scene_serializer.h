#pragma once

#include "engine/scene/scene.h"

namespace susumu
{
    class SceneSerializer
    {
    public:
        SceneSerializer(const Ref<Scene>& scene);
        ~SceneSerializer() = default;

        void Serialize(const std::string& filepath);
        void SerializeRuntime(const std::string& filepath);

        bool Deserialize(const std::string& filepath);
        bool DeserializeRuntime(const std::string& filepath);
    private:
        Ref<Scene> m_Scene;
    };
}

