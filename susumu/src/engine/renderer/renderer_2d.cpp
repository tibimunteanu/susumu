#include "supch.h"
#include "engine/renderer/renderer_2d.h"
#include "engine/renderer/vertex_array.h"
#include "engine/renderer/shader.h"
#include "engine/renderer/render_command.h"

#include <glm/gtc/matrix_transform.hpp>

namespace susumu
{
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;

        //Editor-only
        int EntityID = -1;
    };

    struct Renderer2DData
    {
        static const uint32_t MaxQuads = 50000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 32; //TODO: render caps

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1; //0 = white 1x1 px texture

        glm::vec4 QuadVertexPositions[4] = 
        {
            { -0.5f, -0.5f, 0.0f, 1.0f },
            { 0.5f, -0.5f, 0.0f, 1.0f },
            { 0.5f, 0.5f, 0.0f, 1.0f },
            { -0.5f, 0.5f, 0.0f, 1.0f }
        };

        Renderer2D::Statistics Stats;
    };

    static Renderer2DData s_Data;

    void Renderer2D::Init()
    {
        SU_PROFILE_FUNCTION();

        //create the default 1x1 white texture
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture = Texture2D::Create(1, 1);
        s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
        s_Data.TextureSlots[0] = s_Data.WhiteTexture;

        //load the texture shader
        int32_t samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
        {
            samplers[i] = i;
        }
        s_Data.TextureShader = Shader::Create("assets/shaders/texture.glsl");
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

        //create the quad vertex array
        s_Data.QuadVertexArray = VertexArray::Create();

        //create and add the quad vertex buffer
        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
        s_Data.QuadVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float, "a_TexIndex" },
            { ShaderDataType::Int, "a_EntityID" },
        });
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

        s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

        //create and add the quad index buffer
        uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }
        auto quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
        delete[] quadIndices;
    }

    void Renderer2D::Shutdown()
    {
        SU_PROFILE_FUNCTION();

        delete[] s_Data.QuadVertexBufferBase;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        SU_PROFILE_FUNCTION();

        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        StartBatch();
    }

    void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
    {
        SU_PROFILE_FUNCTION();

        glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

        StartBatch();
    }

    void Renderer2D::BeginScene(const EditorCamera& camera)
    {
        SU_PROFILE_FUNCTION();

        glm::mat4 viewProj = camera.GetViewProjection();

        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

        StartBatch();
    }

    void Renderer2D::EndScene()
    {
        SU_PROFILE_FUNCTION();

        Flush();
    }

    void Renderer2D::StartBatch()
    {
        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        s_Data.TextureSlotIndex = 1;
    }

    void Renderer2D::Flush()
    {
        if (s_Data.QuadIndexCount == 0) return; //nothing to draw

        uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
        {
            s_Data.TextureSlots[i]->Bind(i);
        }
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);

        s_Data.Stats.DrawCalls++;
    }

    void Renderer2D::NextBatch()
    {
        Flush();
        StartBatch();
    }

    void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& sprite, int entityID)
    {
        DrawQuad(transform, sprite.Color, entityID);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotationRad, const glm::vec4& color, int entityID)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, rotationRad, nullptr, color, entityID);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotationRad, const glm::vec4& color, int entityID)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, rotationRad, nullptr, color, entityID);
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
    {
        DrawQuad(transform, nullptr, color, entityID);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotationRad, const Ref<Texture2D>& texture, const glm::vec4& color, int entityID)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, rotationRad, texture, color, entityID);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotationRad, const Ref<Texture2D>& texture, const glm::vec4& color, int entityID)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) //translation
            * glm::rotate(glm::mat4(1.0f), rotationRad, { 0.0f, 0.0f, 1.0f }) //rotation
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }); //scale

        DrawQuad(transform, texture, color, entityID);
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& color, int entityID)
    {
        SU_PROFILE_FUNCTION();

        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 texCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
        {
            NextBatch();
        }

        float texIndex = 0.0f; //white texture
        if (texture)
        {
            for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
            {
                if (*s_Data.TextureSlots[i] == *texture)
                {
                    texIndex = (float)i;
                    break;
                }
            }
            if (texIndex == 0.0f)
            {
                if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
                {
                    NextBatch();
                }

                texIndex = (float)s_Data.TextureSlotIndex;
                s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
                s_Data.TextureSlotIndex++;
            }
        }

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
            s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

        s_Data.QuadIndexCount += 6;

        s_Data.Stats.QuadCount++;
    }

    void Renderer2D::ResetStats()
    {
        memset(&s_Data.Stats, 0, sizeof(Statistics));
    }

    Renderer2D::Statistics Renderer2D::GetStats()
    {
        return s_Data.Stats;
    }
}

