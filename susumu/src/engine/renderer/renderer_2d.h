#pragma once

#include "engine/renderer/camera.h"
#include "engine/renderer/texture.h"

namespace susumu
{
    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();
        static void Flush();

        //primitives
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotationDeg, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotationDeg, const glm::vec4& color);
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotationDeg, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotationDeg, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f));

        //stats
        struct Statistics
        {
            uint32_t DrawCalls = 0;
            uint32_t QuadCount = 0;

            uint32_t GetTotalVertexCount() { return QuadCount * 4; }
            uint32_t GetTotalIndexCount() { return QuadCount * 6; }
        };
        static void ResetStats();
        static Statistics GetStats();

    private:
        static void FlushAndReset();
    };
}
