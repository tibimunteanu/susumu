#pragma once

#include "engine/core/base.h"
#include "engine/renderer/renderer.h"

#include <string>
#include <glm/glm.hpp>

namespace susumu
{
    struct ShaderUniform
    {

    };

    struct ShaderUniformCollection
    {

    };

    enum class UniformType
    {
        None = 0,
        Float, Float2, Float3, Float4,
        Matrix3x3, Matrix4x4,
        Int32, Uint32
    };

    struct UniformDecl
    {
        UniformType Type;
        std::ptrdiff_t Offset;
        std::string Name;
    };

    struct UniformBuffer
    {
        uint8_t* Buffer;
        std::vector<UniformDecl> Uniforms;
    };

    struct UniformBufferBase
    {
        virtual const uint8_t* GetBuffer() const = 0;
        virtual const UniformDecl* GetUniforms() const = 0;
        virtual uint32_t GetUniformCount() const = 0;
    };

    template<uint32_t N, uint32_t U>
    struct UniformBufferDeclaration : public UniformBufferBase
    {
        uint8_t Buffer[N];
        UniformDecl Uniforms[U];
        std::ptrdiff_t Cursor = 0;

        virtual const uint8_t* GetBuffer() const override { return Buffer; }
        virtual const UniformDecl* GetUniforms() const override { return Uniforms; }
        virtual uint32_t GetUniformCount() const override { return U; }

        template<typename T>
        void Push(const std::string& name, const T& data) {}

        template<>
        void Push(const std::string& name, const float& data)
        {
            Uniforms[0] = { UniformType::Float, Cursor, name };
            memcpy(Buffer + Cursor, &data, sizeof(float));
            Cursor += sizeof(float);
        }

        template<>
        void Push(const std::string& name, const glm::vec4& data)
        {
            Uniforms[0] = { UniformType::Float4, Cursor, name };
            memcpy(Buffer + Cursor, glm::value_ptr(data), sizeof(glm::vec4));
            Cursor += sizeof(glm::vec4);
        }
    };

    class Shader
    {
    public:
        virtual void Bind() = 0;
        virtual void UploadUniformBuffer(const UniformBufferBase& uniformBuffer) = 0;

        static Shader* Create(const std::string& filepath);

    };
}
