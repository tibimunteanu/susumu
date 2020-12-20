#include "supch.h"
#include "texture.h"
#include "renderer.h"
#include "platform/opengl/opengl_texture.h"

namespace susumu {

    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None: SU_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);

            default: SU_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
        }
    }

}
