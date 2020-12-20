#include "sandbox_2d.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(960.0f / 540.0f)
{
}

void Sandbox2D::OnAttach()
{
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(susumu::Timestep dt)
{
    ////////////////////////// Update ///////////////////////////////////////////////////////
    m_CameraController.OnUpdate(dt);

    ////////////////////////// Render ///////////////////////////////////////////////////////
    //render a scene
    susumu::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    susumu::RenderCommand::Clear();

    susumu::Renderer2D::BeginScene(m_CameraController.GetCamera());
    {
        susumu::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_SquareColor);
    }
    susumu::Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(susumu::Event& e)
{
    m_CameraController.OnEvent(e);
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

