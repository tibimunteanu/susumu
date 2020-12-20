#include "sandbox_2d.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//TEMP
#include "platform/opengl/opengl_shader.h"

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(960.0f / 540.0f)
{
}

void Sandbox2D::OnAttach()
{
    //load shaders
    m_FlatColorShader = susumu::Shader::Create("assets/shaders/flat_color.glsl");

    //create vertex array
    m_SquareVertexArray = susumu::VertexArray::Create();

    //create and add vertex buffer
    float vertices[3 * 4] =
    {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
    };
    auto vertexBuffer = susumu::VertexBuffer::Create(vertices, sizeof(vertices));
    vertexBuffer->SetLayout({
        { susumu::ShaderDataType::Float3, "a_Position" },
        });
    m_SquareVertexArray->AddVertexBuffer(vertexBuffer);

    //create and add index buffer
    uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
    auto indexBuffer = susumu::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    m_SquareVertexArray->SetIndexBuffer(indexBuffer);
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

    susumu::Renderer::BeginScene(m_CameraController.GetCamera());
    {
        std::dynamic_pointer_cast<susumu::OpenGLShader>(m_FlatColorShader)->Bind();
        std::dynamic_pointer_cast<susumu::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

        susumu::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
    }
    susumu::Renderer::EndScene();
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

