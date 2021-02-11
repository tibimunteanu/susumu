#pragma once

#include "susumu.h"
#include "engine/renderer/editor_camera.h"
#include "panels/scene_hierarchy_panel.h"

namespace susumu
{
    class EditorLayer : public Layer
    {
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep dt) override;
        void OnEvent(Event& e) override;
        void OnImGuiRender() override;
    private:
        bool OnKeyPressed(KeyPressedEvent e);
        void NewScene();
        void OpenScene();
        void SaveSceneAs();
    private:
        OrthographicCameraController m_CameraController;

        //TEMP
        Ref<Texture2D> m_SpaceTexture;
        Ref<Texture2D> m_EarthTexture;
        Ref<Framebuffer> m_Framebuffer;

        Ref<Scene> m_ActiveScene;
        Entity m_SquareEntity;
        Entity m_CameraEntity;
        Entity m_HoveredEntity;

        EditorCamera m_EditorCamera;

        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        glm::vec2 m_ViewportBounds[2];
        bool m_ViewportFocused = false;
        bool m_ViewportHovered = false;

        struct ProfileResult
        {
            const char* Name;
            float Time;
        };
        std::vector<ProfileResult> m_ProfileResults;

        int m_GizmoType = -1;

        //panels
        SceneHierarchyPanel m_SceneHierarchyPanel;
    };
}
