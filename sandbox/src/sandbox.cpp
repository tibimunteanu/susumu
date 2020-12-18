#include <susumu.h>

class ExampleLayer : public susumu::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
    {
        float vertices[3 * 7] =
        {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        };
        std::shared_ptr<susumu::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(susumu::VertexBuffer::Create(vertices, sizeof(vertices)));
        vertexBuffer->SetLayout({
            { susumu::ShaderDataType::Float3, "a_Position" },
            { susumu::ShaderDataType::Float4, "a_Color" }
        });

        m_VertexArray.reset(susumu::VertexArray::Create());
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = { 0, 1, 2 };
        std::shared_ptr<susumu::IndexBuffer> indexBuffer;
        indexBuffer.reset(susumu::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        std::string vertexSource = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;

            out vec3 v_Position;
            out vec4 v_Color;
            
            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSource = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            in vec4 v_Color;
            
            void main()
            {
                color = v_Color;
            }
        )";

        m_Shader.reset(new susumu::Shader(vertexSource, fragmentSource));
    }

    void OnUpdate() override
    {
        if (susumu::Input::IsKeyPressed(SU_KEY_LEFT)) m_CameraPosition.x -= m_CameraMoveSpeed;
        else if (susumu::Input::IsKeyPressed(SU_KEY_RIGHT)) m_CameraPosition.x += m_CameraMoveSpeed;

        if (susumu::Input::IsKeyPressed(SU_KEY_DOWN)) m_CameraPosition.y -= m_CameraMoveSpeed;
        else if (susumu::Input::IsKeyPressed(SU_KEY_UP)) m_CameraPosition.y += m_CameraMoveSpeed;

        if (susumu::Input::IsKeyPressed(SU_KEY_SPACE)) m_CameraRotation -= m_CameraRotationSpeed;
        else if (susumu::Input::IsKeyPressed(SU_KEY_LEFT_SHIFT)) m_CameraRotation += m_CameraRotationSpeed;

        susumu::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        susumu::RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        susumu::Renderer::BeginScene(m_Camera);
        {
            susumu::Renderer::Submit(m_Shader, m_VertexArray);
        }
        susumu::Renderer::EndScene();
    }

    void OnEvent(susumu::Event& event) override
    {
    }

private:
    std::shared_ptr<susumu::Shader> m_Shader;
    std::shared_ptr<susumu::VertexArray> m_VertexArray;

    susumu::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition;
    float m_CameraMoveSpeed = 0.1f;
    float m_CameraRotation = 0.0f;
    float m_CameraRotationSpeed = 2.0f;
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
