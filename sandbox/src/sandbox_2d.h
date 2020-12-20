#pragma once

#include "susumu.h"

class Sandbox2D : public susumu::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(susumu::Timestep dt) override;
    void OnEvent(susumu::Event& e) override;
    void OnImGuiRender() override;

private:
    susumu::OrthographicCameraController m_CameraController;

    //TEMP
    susumu::Ref<susumu::Shader> m_FlatColorShader;
    susumu::Ref<susumu::VertexArray> m_SquareVertexArray;

    glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

};
