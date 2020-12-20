#include "supch.h"
#include "texture.h"
#include "renderer.h"
#include "platform/opengl/opengl_texture.h"

namespace susumu
{
    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None: SU_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(width, height);

            default: SU_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
        }
    }

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
