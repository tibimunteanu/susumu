#include "editor_layer.h"
#include "engine/scene/scene_serializer.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace susumu
{
    EditorLayer::EditorLayer()
        : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f)
    {
    }

    void EditorLayer::OnAttach()
    {
        SU_PROFILE_FUNCTION();

        //set vsync
        App::Get().GetWindow().SetVSync(true);

        //load some textures
        m_SpaceTexture = Texture2D::Create("assets/textures/space.png");
        m_EarthTexture = Texture2D::Create("assets/textures/earth.png");

        FramebufferSpec fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = Framebuffer::Create(fbSpec);

        m_ActiveScene = CreateRef<Scene>();
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);

    #if 0
        m_SquareEntity = m_ActiveScene->CreateEntity("Square");
        m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.3f, 0.8f, 0.2f, 1.0f });

        m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
        m_CameraEntity.AddComponent<CameraComponent>();

        class CameraController : public ScriptableEntity
        {
        public:
            void OnCreate()
            {
            }

            void OnDestroy()
            {
            }

            void OnUpdate(Timestep dt)
            {
                auto& position = GetComponent<TransformComponent>().Translation;
                float speed = 5.0f;

                if (Input::IsKeyPressed(Key::A)) position.x -= speed * dt;
                if (Input::IsKeyPressed(Key::D)) position.x += speed * dt;
                if (Input::IsKeyPressed(Key::W)) position.y += speed * dt;
                if (Input::IsKeyPressed(Key::S)) position.y -= speed * dt;
            }
        };

        m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

    #endif
    }

    void EditorLayer::OnDetach()
    {
        SU_PROFILE_FUNCTION();
    }

    void EditorLayer::OnUpdate(Timestep dt)
    {
        SU_PROFILE_FUNCTION();

        //handle resize
        FramebufferSpec spec = m_Framebuffer->GetSpec();
        if (m_ViewportSize.x > 0.0f 
            && m_ViewportSize.y > 0.0f
            && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        ////////////////////////// Update ///////////////////////////////////////////////////////
        if (m_ViewportFocused)
        {
            m_CameraController.OnUpdate(dt);
        }

        ////////////////////////// Render ///////////////////////////////////////////////////////
        //render a scene
        Renderer2D::ResetStats();
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        RenderCommand::Clear();
        m_ActiveScene->OnUpdate(dt);
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
            ImGuiStyle& style = ImGui::GetStyle();
            //float minWinSizeX = style.WindowMinSize.x;
            //style.WindowMinSize.x = 360.0f;

            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }
            //style.WindowMinSize.x = minWinSizeX;

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Serialize"))
                    {
                        SceneSerializer serializer(m_ActiveScene);
                        serializer.Serialize("assets/scenes/ExampleScene.su");
                    }

                    if (ImGui::MenuItem("Deserialize"))
                    {
                        SceneSerializer serializer(m_ActiveScene);
                        serializer.Deserialize("assets/scenes/ExampleScene.su");
                    }

                    if (ImGui::MenuItem("Exit"))
                    {
                        App::Get().Close();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            m_SceneHierarchyPanel.OnImGuiRender();

            ImGui::Begin("Console");
            {

            }
            ImGui::End();

            ImGui::Begin("Stats");
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
            }
            ImGui::End();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Begin("Scene");
            {
                m_ViewportFocused = ImGui::IsWindowFocused();
                m_ViewportHovered = ImGui::IsWindowHovered();
                App::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused && !m_ViewportHovered);

                ImVec2 viewportSize = ImGui::GetContentRegionAvail();
                m_ViewportSize = { viewportSize.x, viewportSize.y };
                ImGui::Image(reinterpret_cast<void*>(m_Framebuffer->GetColorAttachmentRendererID()), ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
            }
            ImGui::End();
            ImGui::PopStyleVar();
        }
        ImGui::End();
    }
}
