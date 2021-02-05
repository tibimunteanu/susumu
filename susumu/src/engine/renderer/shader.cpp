#include "supch.h"
#include "engine/renderer/shader.h"
#include "platform/opengl/opengl_shader.h"

namespace susumu
{
    Shader* Shader::Create(const std::string& filepath)
    {
        switch (RendererAPI::Current())
        {
            case RendererAPIType::None: return nullptr;
            case RendererAPIType::OpenGL: return new OpenGLShader(filepath);
            default: return nullptr;
        }
    }
}
