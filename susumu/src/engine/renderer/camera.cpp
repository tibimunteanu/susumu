#include "supch.h"
#include "engine/renderer/camera.h"
#include "engine/core/input.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#define M_PI 3.1415926536897f

namespace susumu
{
    Camera::Camera(const glm::mat4& projectionMatrix)
        : m_ProjectionMatrix(projectionMatrix)
    {
        // Sensible defaults
        m_PanSpeed = 0.15f;
        m_RotationSpeed = 0.3f;
        m_ZoomSpeed = 1.0f;

        m_Position = { -5, 5, 5 };
        m_Rotation = glm::vec3(90.0f, 0.0f, 0.0f);

        m_FocalPoint = glm::vec3(0.0f);
        m_Distance = glm::distance(m_Position, m_FocalPoint);

        m_Yaw = 3.0f * (float)M_PI / 4.0f;
        m_Pitch = M_PI / 4.0f;
    }

    void Camera::Focus()
    {
    }

    void Camera::Update(Timestep ts)
    {
        if (Input::IsKeyPressed(Key::LeftAlt))
        {
            const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
            glm::vec2 delta = mouse - m_InitialMousePosition;
            m_InitialMousePosition = mouse;

            delta *= ts.GetSeconds();

            if (Input::IsMouseButtonPressed(Mouse::Button2))
                MousePan(delta);
            else if (Input::IsMouseButtonPressed(Mouse::Button0))
                MouseRotate(delta);
            else if (Input::IsMouseButtonPressed(Mouse::Button1))
                MouseZoom(delta.y);
        }

        m_Position = CalculatePosition();

        glm::quat orientation = GetOrientation();
        m_Rotation = glm::eulerAngles(orientation) * (180.0f / (float)M_PI);
        m_ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 1)) * glm::toMat4(glm::conjugate(orientation)) * glm::translate(glm::mat4(1.0f), -m_Position);
        m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
        m_ViewMatrix = glm::inverse(m_ViewMatrix);
    }

    void Camera::MousePan(const glm::vec2& delta)
    {
        m_FocalPoint += -GetRightDirection() * delta.x * m_PanSpeed * m_Distance;
        m_FocalPoint += GetUpDirection() * delta.y * m_PanSpeed * m_Distance;
    }

    void Camera::MouseRotate(const glm::vec2& delta)
    {
        float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
        m_Yaw += yawSign * delta.x * m_RotationSpeed;
        m_Pitch += delta.y * m_RotationSpeed;
    }

    void Camera::MouseZoom(float delta)
    {
        m_Distance -= delta * m_ZoomSpeed;
        if (m_Distance < 1.0f)
        {
            m_FocalPoint += GetForwardDirection();
            m_Distance = 1.0f;
        }
    }

    glm::vec3 Camera::GetUpDirection()
    {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 Camera::GetRightDirection()
    {
        return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 Camera::GetForwardDirection()
    {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::vec3 Camera::CalculatePosition()
    {
        return m_FocalPoint - GetForwardDirection() * m_Distance;
    }

    glm::quat Camera::GetOrientation()
    {
        return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
    }
}