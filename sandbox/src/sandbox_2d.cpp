#include "sandbox_2d.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(960.0f / 540.0f)
{
}

void Sandbox2D::OnAttach()
{
    SU_PROFILE_FUNCTION();

    //load some textures
    m_SpaceTexture = susumu::Texture2D::Create("assets/textures/space.png");
    m_EarthTexture = susumu::Texture2D::Create("assets/textures/earth.png");
}

void Sandbox2D::OnDetach()
{
    SU_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(susumu::Timestep dt)
{
    SU_PROFILE_FUNCTION();

    SU_TRACE("Frame time: {0}, FPS {1}", dt, 1.0f / dt);

    ////////////////////////// Update ///////////////////////////////////////////////////////
    m_CameraController.OnUpdate(dt);

    ////////////////////////// Render ///////////////////////////////////////////////////////
    //render a scene
    susumu::Renderer2D::ResetStats();
    susumu::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    susumu::RenderCommand::Clear();

    {
        SU_PROFILE_SCOPE("Renderer Draw");
        susumu::Renderer2D::BeginScene(m_CameraController.GetCamera());
        {
            static float rot = 0.0f;
            rot += dt * 2.0f;

            susumu::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, 0.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
            susumu::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, 0.0f, m_SquareColor);
            susumu::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 2.0f, 2.0f }, 0.0f, m_SpaceTexture, { 1.0f, 0.5f, 0.5f, 1.0f });
            susumu::Renderer2D::DrawQuad({ 0.2f, 0.5f }, { 1.0f, 1.0f }, rot, m_EarthTexture);
        }
        susumu::Renderer2D::EndScene();

        susumu::Renderer2D::BeginScene(m_CameraController.GetCamera());
        {
            for (float y = -5.0f; y < 5.0f; y += 0.5f)
            {
                for (float x = -5.0f; x < 5.0f; x += 0.5f)
                {
                    glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 1.0f };
                    susumu::Renderer2D::DrawQuad({ x, y, -0.1f }, { 0.5f, 0.5f }, 0.0f, nullptr, color);
                }
            }
        }
        susumu::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnEvent(susumu::Event& e)
{
    m_CameraController.OnEvent(e);
}

void Sandbox2D::OnImGuiRender()
{
    SU_PROFILE_FUNCTION();

    ImGui::Begin("Settings");

    auto stats = susumu::Renderer2D::GetStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

    ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor));

    ImGui::End();
}

