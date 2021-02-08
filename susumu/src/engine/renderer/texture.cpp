#include "supch.h"
#include "engine/renderer/texture.h"
#include "engine/renderer/renderer_api.h"
#include "platform/opengl/opengl_texture.h"

namespace susumu
{
    Texture2D* Texture2D::Create(TextureFormat format, uint32_t width, uint32_t height, TextureWrap wrap)
    {
        switch (RendererAPI::Current())
        {
            case RendererAPIType::None: return nullptr;
            case RendererAPIType::OpenGL: return new OpenGLTexture2D(format, width, height, wrap);
            default: return nullptr;
        }
    }

    Texture2D* Texture2D::Create(const std::string& path, bool srgb)
    {
        switch (RendererAPI::Current())
        {
            case RendererAPIType::None: return nullptr;
            case RendererAPIType::OpenGL: return new OpenGLTexture2D(path, srgb);
            default: return nullptr;
        }
    }

    TextureCube* TextureCube::Create(const std::string& path)
    {
        switch (RendererAPI::Current())
        {
            case RendererAPIType::None: return nullptr;
            case RendererAPIType::OpenGL: return new OpenGLTextureCube(path);
            default: return nullptr;
        }
    }

    uint32_t Texture::GetBPP(TextureFormat format)
    {
        switch (format)
        {
            case TextureFormat::RGB:    return 3;
            case TextureFormat::RGBA:   return 4;
        }
        return 0;
    }

}
