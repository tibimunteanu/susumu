#include "editor_layer.h"
#include "engine/scene/scene_serializer.h"
#include "engine/utils/platform_utils.h"
#include "engine/math/math.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <ImGuizmo.h>

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
        fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = Framebuffer::Create(fbSpec);

        m_ActiveScene = CreateRef<Scene>();
        m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

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
            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        ////////////////////////// Update ///////////////////////////////////////////////////////
        if (m_ViewportFocused)
        {
            m_CameraController.OnUpdate(dt);
        }
        m_EditorCamera.OnUpdate(dt);

        ////////////////////////// Render ///////////////////////////////////////////////////////
        //render a scene
        Renderer2D::ResetStats();
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
        RenderCommand::Clear();

        //Clear entityID attachment to -1
        m_Framebuffer->ClearAttachment(1, -1);

        m_ActiveScene->OnUpdateEditor(dt, m_EditorCamera);

        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;
        glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
        my = viewportSize.y - my;
        int mouseX = (int)mx;
        int mouseY = (int)my;
        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
        {
            int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
            m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
        }

        m_Framebuffer->Unbind();
    }

    void EditorLayer::OnEvent(Event& e)
    {
        m_CameraController.OnEvent(e);
        m_EditorCamera.OnEvent(e);

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(SU_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    }

    void EditorLayer::OnImGuiRender()
    {
        SU_PROFILE_FUNCTION();

        static bool dockspaceOpen = true;
        static bool opt_fullscreen_persistent = true;
        bool opt_fullscreen = opt_fullscreen_persistent;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
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

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();
        {
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
                    if (ImGui::MenuItem("New", "Ctrl+N")) NewScene();
                    if (ImGui::MenuItem("Open...", "Ctrl+O")) OpenScene();
                    if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) SaveSceneAs();

                    if (ImGui::MenuItem("Exit")) App::Get().Close();

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
                ImGui::Text("Hovered Entity: %s", (m_HoveredEntity ? m_HoveredEntity.GetComponent<TagComponent>().Tag : "None").c_str());

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
            ImGui::Begin("Scene", 0, ImGuiWindowFlags_NoTitleBar);
            {
                m_ViewportFocused = ImGui::IsWindowFocused();
                m_ViewportHovered = ImGui::IsWindowHovered();
                App::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused && !m_ViewportHovered);

                //compute panel viewport bounds and size
                ImVec2 panelPos = ImGui::GetWindowPos();
                ImVec2 minBound = ImGui::GetWindowContentRegionMin();
                ImVec2 maxBound = ImGui::GetWindowContentRegionMax();
                minBound.x += panelPos.x;
                minBound.y += panelPos.y;
                maxBound.x += panelPos.x;
                maxBound.y += panelPos.y;
                m_ViewportSize = { maxBound.x - minBound.x, maxBound.y - minBound.y };
                m_ViewportBounds[0] = { minBound.x, minBound.y };
                m_ViewportBounds[1] = { maxBound.x, maxBound.y };

                //draw scene texture
                uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID(0);
                ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

                //Gizmos
                Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
                if (selectedEntity && m_GizmoType != -1)
                {
                    ImGuizmo::SetOrthographic(false);
                    ImGuizmo::SetDrawlist();

                    float windowWidth = (float)ImGui::GetWindowWidth();
                    float windowHeight = (float)ImGui::GetWindowHeight();
                    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

                    // runtime camera
                    //auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
                    //const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
                    //const glm::mat4& cameraProjection = camera.GetProjection();
                    //glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

                    // editor camera
                    const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
                    glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

                    // selected entity
                    auto& tc = selectedEntity.GetComponent<TransformComponent>();
                    glm::mat4 transform = tc.GetTransform();

                    // snapping
                    bool snap = Input::IsKeyPressed(Key::LeftControl);
                    float snapValue = m_GizmoType == ImGuizmo::OPERATION::ROTATE ? 45.0f : 0.5f;
                    float snapValues[3] = { snapValue, snapValue, snapValue };

                    ImGuizmo::Manipulate(glm::value_ptr(cameraView),
                        glm::value_ptr(cameraProjection),
                        (ImGuizmo::OPERATION)m_GizmoType,
                        ImGuizmo::LOCAL,
                        glm::value_ptr(transform),
                        nullptr, snap ? snapValues : nullptr);

                    if (ImGuizmo::IsUsing())
                    {
                        glm::vec3 translation, rotation, scale;
                        math::DecomposeTransform(transform, translation, rotation, scale);

                        glm::vec3 deltaRotation = rotation - tc.Rotation;
                        tc.Translation = translation;
                        tc.Rotation += deltaRotation;
                        tc.Scale = scale;
                    }
                }
            }
            ImGui::End();
            ImGui::PopStyleVar();
        }
        ImGui::End();
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent e)
    {
        //shortcuts
        if (e.GetRepeatCount() > 0) return false;
        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
        switch (e.GetKeyCode())
        {
            case Key::N:
            {
                if (control)
                {
                    NewScene();
                }
            }
            break;
            case Key::O:
            {
                if (control)
                {
                    OpenScene();
                }
            }
            break;
            case Key::S:
            {
                if (control && shift)
                {
                    SaveSceneAs();
                }
            }
            break;

            //gizmos
            case Key::Q:
            {
                m_GizmoType = -1;
            }
            break;
            case Key::W:
            {
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            }
            break;
            case Key::E:
            {
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            }
            break;
            case Key::R:
            {
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
            }
            break;

            default: break;
        }
    }

    void EditorLayer::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OpenScene()
    {
        std::string filepath = FileDialogs::OpenFile("Susumu Scene (*.su)\0*.su\0)");
        if (!filepath.empty())
        {
            NewScene();

            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(filepath);
        }
    }

    void EditorLayer::SaveSceneAs()
    {
        std::string filepath = FileDialogs::SaveFile("Susumu Scene (*.su)\0*.su\0)");
        if (!filepath.empty())
        {
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(filepath);
        }
    }
}
