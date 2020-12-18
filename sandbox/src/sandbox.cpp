#include <susumu.h>

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//TEMP
#include "platform/opengl/opengl_shader.h"

class ExampleLayer : public susumu::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
    {
        std::string vertexSource = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_Position;
            
            void main()
            {
                v_Position = a_Position;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";
        std::string fragmentSource = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            uniform vec3 u_Color;

            in vec3 v_Position;
            
            void main()
            {
                color = vec4(u_Color, 1.0);
            }
        )";
        m_Shader.reset(susumu::Shader::Create(vertexSource, fragmentSource));

        float vertices[3 * 4] =
        {
            -0.5f, -0.5f, 0.0f, 
            0.5f, -0.5f, 0.0f, 
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f
        };
        std::shared_ptr<susumu::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(susumu::VertexBuffer::Create(vertices, sizeof(vertices)));
        vertexBuffer->SetLayout({
            { susumu::ShaderDataType::Float3, "a_Position" }
        });

        m_VertexArray.reset(susumu::VertexArray::Create());
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
        std::shared_ptr<susumu::IndexBuffer> indexBuffer;
        indexBuffer.reset(susumu::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);
    }

    void OnUpdate(susumu::Timestep dt) override
    {
        //SU_CORE_TRACE("{0}s, {1}ms", dt.GetSeconds(), dt.GetMilliseconds());

        if (susumu::Input::IsKeyPressed(SU_KEY_LEFT)) m_CameraPosition.x -= m_CameraMoveSpeed * dt;
        else if (susumu::Input::IsKeyPressed(SU_KEY_RIGHT)) m_CameraPosition.x += m_CameraMoveSpeed * dt;

        if (susumu::Input::IsKeyPressed(SU_KEY_DOWN)) m_CameraPosition.y -= m_CameraMoveSpeed * dt;
        else if (susumu::Input::IsKeyPressed(SU_KEY_UP)) m_CameraPosition.y += m_CameraMoveSpeed * dt;

        if (susumu::Input::IsKeyPressed(SU_KEY_SPACE)) m_CameraRotation -= m_CameraRotationSpeed * dt;
        else if (susumu::Input::IsKeyPressed(SU_KEY_LEFT_SHIFT)) m_CameraRotation += m_CameraRotationSpeed * dt;

        susumu::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        susumu::RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        susumu::Renderer::BeginScene(m_Camera);
        {
            std::dynamic_pointer_cast<susumu::OpenGLShader>(m_Shader)->Bind();
            std::dynamic_pointer_cast<susumu::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_Color);

            glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

            for (int y = -10; y < 10; y++)
            {
                for (int x = -12; x < 12; x++)
                {
                    glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                    glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
                    susumu::Renderer::Submit(m_Shader, m_VertexArray, transform);
                }
            }
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
    }

private:
    std::shared_ptr<susumu::Shader> m_Shader;
    std::shared_ptr<susumu::VertexArray> m_VertexArray;

    susumu::OrthographicCamera m_Camera;

    glm::vec3 m_Color = { 0.2f, 0.3f, 0.8f };
    glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
    float m_CameraRotation = 0.0f;

    float m_CameraMoveSpeed = 2.0f;
    float m_CameraRotationSpeed = 90.0f;
};

class Sandbox : public susumu::App
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
    }

    ~Sandbox()
    {

    }
};

susumu::App* susumu::CreateApp()
{
    return new Sandbox();
}
