#include "supch.h"
#include "shader.h"

#include "renderer.h"
#include "platform/opengl/opengl_shader.h"

namespace susumu {

    ////////////////////////////////////////////////////////////////////////////////
    // Shader //////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    Ref<Shader> Shader::Create(const std::string& filepath)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None: SU_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(filepath);

            default: SU_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
        }
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string& filepath)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None: SU_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(name, filepath);

            default: SU_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
        }
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None: SU_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);

            default: SU_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
        }
    }


    ////////////////////////////////////////////////////////////////////////////////
    // ShaderLibrary ///////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    void ShaderLibrary::Add(const Ref<Shader>& shader)
    {
        auto& name = shader->GetName();
        SU_CORE_ASSERT(!Exists(name), "Shader '{0}' already exists!", name);
        m_Shaders[name] = shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string & filepath)
    {
        auto shader = Shader::Create(name, filepath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Get(const std::string& name)
    {
        SU_CORE_ASSERT(Exists(name), "Shader '{0}' not found!", name);
        return m_Shaders[name];
    }

    bool ShaderLibrary::Exists(const std::string& name) const
    {
        return m_Shaders.find(name) != m_Shaders.end();
    }

}
