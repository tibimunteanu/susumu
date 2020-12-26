#pragma once

#include "engine/renderer/camera.h"
#include "engine/renderer/editor_camera.h"
#include "engine/renderer/texture.h"

namespace susumu
{
    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const Camera& camera, const glm::mat4& transform);
        static void BeginScene(const EditorCamera& camera);
        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();
        static void Flush();

        //primitives
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotationRad, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotationRad, const glm::vec4& color);
        static void DrawQuad(const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotationRad, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotationRad, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f));
        static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f));

        //stats
        struct Statistics
        {
            uint32_t DrawCalls = 0;
            uint32_t QuadCount = 0;

            uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
            uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
        };
        static void ResetStats();
        static Statistics GetStats();

    private:
        static void StartBatch();
        static void NextBatch();
    };
}
