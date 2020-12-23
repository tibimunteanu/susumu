#pragma once

#include "engine/renderer/camera.h"

namespace susumu
{
    class SceneCamera : public Camera
    {
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void SetViewportSize(uint32_t width, uint32_t height);
        void SetOrthographic(float size, float nearClip, float farClip);

        float GetOrthographicSize() const { return m_OrthographicSize; }
        void SetOrthographicSize(float size) { m_OrthographicSize = size; RecomputeProjection(); }
    private:
        void RecomputeProjection();
    private:
        float m_OrthographicSize = 10.0f;
        float m_OrthographicNear = -1.0f;
        float m_OrthographicFar = 1.0f;
        float m_AspectRatio = 0.0f;
    };
}
