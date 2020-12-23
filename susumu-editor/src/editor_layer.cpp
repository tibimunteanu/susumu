#include "editor_layer.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace susumu
{
    EditorLayer::EditorLayer()
        : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
    {
    }

    void EditorLayer::OnAttach()
    {
        SU_PROFILE_FUNCTION();

        //load some textures
        m_SpaceTexture = Texture2D::Create("assets/textures/space.png");
        m_EarthTexture = Texture2D::Create("assets/textures/earth.png");

        FramebufferSpec fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = Framebuffer::Create(fbSpec);

        //set vsync
        App::Get().GetWindow().SetVSync(true);
    }

    void EditorLayer::OnDetach()
    {
        SU_PROFILE_FUNCTION();
    }

    void EditorLayer::OnUpdate(Timestep dt)
    {
        SU_PROFILE_FUNCTION();

        ////////////////////////// Update ///////////////////////////////////////////////////////
        m_CameraController.OnUpdate(dt);

        ////////////////////////// Render ///////////////////////////////////////////////////////
        //render a scene
        Renderer2D::ResetStats();
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        RenderCommand::Clear();

        {
            SU_PROFILE_SCOPE("Renderer Draw");
            Renderer2D::BeginScene(m_CameraController.GetCamera());
            {
                static float rot = 0.0f;
                rot += dt * 0.05f;

                Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, 0.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
                Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, 0.0f, m_SquareColor);
                Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 2.0f, 2.0f }, 0.0f, m_SpaceTexture, { 1.0f, 0.5f, 0.5f, 1.0f });
                Renderer2D::DrawQuad({ 0.2f, 0.5f }, { 1.0f, 1.0f }, rot, m_EarthTexture);
            }
            Renderer2D::EndScene();

            Renderer2D::BeginScene(m_CameraController.GetCamera());
            {
                for (float y = -5.0f; y < 5.0f; y += 0.5f)
                {
                    for (float x = -5.0f; x < 5.0f; x += 0.5f)
                    {
                        glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 1.0f };
                        Renderer2D::DrawQuad({ x, y, -0.1f }, { 0.5f, 0.5f }, 0.0f, nullptr, color);
                    }
                }
            }
            Renderer2D::EndScene();
        }
        m_Framebuffer->Unbind();
    }

    void EditorLayer::OnEvent(Event& e)
    {
        m_CameraController.OnEvent(e);
    }

    void EditorLayer::OnImGuiRender()
    {
        SU_PROFILE_FUNCTION();

        static bool dockspaceOpen = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->GetWorkPos());
            ImGui::SetNextWindowSize(viewport->GetWorkSize());
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        {
            window_flags |= ImGuiWindowFlags_NoBackground;
        }
        if (!opt_padding)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        }

        ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
        {
            if (!opt_padding)
            {
                ImGui::PopStyleVar();
            }
            if (opt_fullscreen)
            {
                ImGui::PopStyleVar(2);
            }

            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Exit", ""))
                    {
                        App::Get().Close();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            ImGui::Begin("Renderer2D Stats");
            {
                auto stats = Renderer2D::GetStats();
                App& app = App::Get();
                float dt = app.GetLastTimeStep();
                ImGui::Text("FPS: %.0f", 1.0f / dt);
                ImGui::Text("Frame Time: %.4f ms", dt);
                ImGui::Text("VSync: %s", app.GetWindow().IsVSync() ? "On" : "Off");
                ImGui::Text("Draw Calls: %d", stats.DrawCalls);
                ImGui::Text("Quads: %d", stats.QuadCount);
                ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
                ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
                ImGui::End();

                ImGui::Begin("Properties");
                ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor));
            }
            ImGui::End();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Begin("Scene");
            {
                ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
                if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
                {
                    m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
                    m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
                    m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
                }
                ImGui::Image((void*)m_Framebuffer->GetColorAttachmentRendererID(), ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
            }
            ImGui::End();
            ImGui::PopStyleVar();
        }
        ImGui::End();
    }
}
