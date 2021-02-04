#include "supch.h"
#include "engine/renderer/texture.h"
#include "engine/renderer/renderer_api.h"
#include "platform/opengl/opengl_texture.h"

namespace susumu
{
    Texture2D* Texture2D::Create(TextureFormat format, unsigned int width, unsigned int height)
    {
        switch (RendererAPI::Current())
        {
            case RendererAPIType::None: return nullptr;
            case RendererAPIType::OpenGL: return new OpenGLTexture2D(format, width, height);
            default: return nullptr;
        }
    }
}
