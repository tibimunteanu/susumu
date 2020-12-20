#include "supch.h"
#include "renderer_2d.h"

#include "vertex_array.h"
#include "shader.h"
#include "render_command.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace susumu
{
    struct Renderer2DStorage
    {
        Ref<VertexArray> QuadVertexArray;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;
    };

    static Renderer2DStorage* s_Data;

    void Renderer2D::Init()
    {
        s_Data = new Renderer2DStorage();

        //create default 1x1 white texture
        uint32_t whiteTextureData = 0xffffffff;
        s_Data->WhiteTexture = Texture2D::Create(1, 1);
        s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        //load shaders
        s_Data->TextureShader = Shader::Create("assets/shaders/texture.glsl");
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetInt("u_Texture", 0);

        //create vertex array
        s_Data->QuadVertexArray = VertexArray::Create();

        //create and add vertex buffer
        float vertices[5 * 4] =
        {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
        };
        auto vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
        vertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" },
        });
        s_Data->QuadVertexArray->AddVertexBuffer(vertexBuffer);

        //create and add index buffer
        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
        auto indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
        s_Data->QuadVertexArray->SetIndexBuffer(indexBuffer);
    }

    void Renderer2D::Shutdown()
    {
        delete s_Data;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    }

    void Renderer2D::EndScene()
    {
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, nullptr, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, nullptr, color);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color)
    {
        if (texture)
        {
            texture->Bind();
        }
        else
        {
            s_Data->WhiteTexture->Bind();
        }
        
        s_Data->TextureShader->SetFloat4("u_Color", color);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) //translation
            //rotation
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }); //scale
        s_Data->TextureShader->SetMat4("u_Transform", transform);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }
}

