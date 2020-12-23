#pragma once

#include "susumu.h"
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
        OrthographicCameraController m_CameraController;

        //TEMP
        Ref<Texture2D> m_SpaceTexture;
        Ref<Texture2D> m_EarthTexture;
        Ref<Framebuffer> m_Framebuffer;

        glm::vec4 m_SquareColor;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

        struct ProfileResult
        {
            const char* Name;
            float Time;
        };
        std::vector<ProfileResult> m_ProfileResults;
    };
}
