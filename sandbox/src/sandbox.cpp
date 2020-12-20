#include <susumu.h>
#include <engine/core/startup.h>

#include "sandbox_2d.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//TEMP
#include "platform/opengl/opengl_shader.h"

class ExampleLayer : public susumu::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_CameraController(960.0f / 540.0f)
    {
        //load textures
        m_SpaceTexture = susumu::Texture2D::Create("assets/textures/space.png");
        m_EarthTexture = susumu::Texture2D::Create("assets/textures/earth.png");

        //load shaders
        auto flatColorShader = m_ShaderLibrary.Load("assets/shaders/flat_color.glsl");
        auto textureShader = m_ShaderLibrary.Load("assets/shaders/texture.glsl");

        //create vertex array
        m_VertexArray = susumu::VertexArray::Create();

        //create and add vertex buffer
        float vertices[5 * 4] =
        {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
        };
        auto vertexBuffer = susumu::VertexBuffer::Create(vertices, sizeof(vertices));
        vertexBuffer->SetLayout({
            { susumu::ShaderDataType::Float3, "a_Position" },
            { susumu::ShaderDataType::Float2, "a_TexCoord" }
        });
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        //create and add index buffer
        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
        auto indexBuffer = susumu::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
        m_VertexArray->SetIndexBuffer(indexBuffer);
    }

    void OnUpdate(susumu::Timestep dt) override
    {
        //SU_CORE_TRACE("{0}s, {1}ms", dt.GetSeconds(), dt.GetMilliseconds());

        ////////////////////////// Update ///////////////////////////////////////////////////////
        m_CameraController.OnUpdate(dt);

        ////////////////////////// Render ///////////////////////////////////////////////////////
        //render a scene
        susumu::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        susumu::RenderCommand::Clear();

        susumu::Renderer::BeginScene(m_CameraController.GetCamera());
        {
            //set uniforms
            auto flatColorShader = m_ShaderLibrary.Get("flat_color");
            auto textureShader = m_ShaderLibrary.Get("texture");
            std::dynamic_pointer_cast<susumu::OpenGLShader>(flatColorShader)->Bind();
            std::dynamic_pointer_cast<susumu::OpenGLShader>(flatColorShader)->UploadUniformFloat3("u_Color", m_Color);
            std::dynamic_pointer_cast<susumu::OpenGLShader>(textureShader)->Bind();
            std::dynamic_pointer_cast<susumu::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);

            //submit grid of squares
            for (int y = -10; y < 10; y++)
            {
                for (int x = -12; x < 12; x++)
                {
                    glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                    glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
                    susumu::Renderer::Submit(flatColorShader, m_VertexArray, transform);
                }
            }

            //submit space texture
            m_SpaceTexture->Bind(0);
            susumu::Renderer::Submit(textureShader, m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

            //submit earth texture
            m_EarthTexture->Bind(0);
            susumu::Renderer::Submit(textureShader, m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));
        }
        susumu::Renderer::EndScene();
    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square color", glm::value_ptr(m_Color));
        ImGui::End();
    }

    void OnEvent(susumu::Event& event) override
    {
        m_CameraController.OnEvent(event);
    }

private:
    susumu::OrthographicCameraController m_CameraController;
    susumu::ShaderLibrary m_ShaderLibrary;

    susumu::Ref<susumu::VertexArray> m_VertexArray;
    susumu::Ref<susumu::Texture2D> m_SpaceTexture;
    susumu::Ref<susumu::Texture2D> m_EarthTexture;

    glm::vec3 m_Color = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public susumu::App
{
public:
    Sandbox()
    {
        //PushLayer(new ExampleLayer());
        PushLayer(new Sandbox2D());
    }

    ~Sandbox()
    {

    }
};

susumu::App* susumu::CreateApp()
{
    return new Sandbox();
}
