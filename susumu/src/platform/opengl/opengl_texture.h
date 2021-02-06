#pragma once

#include "engine/renderer/renderer_api.h"
#include "engine/renderer/texture.h"

namespace susumu
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(TextureFormat format, uint32_t width, uint32_t height);
        ~OpenGLTexture2D();

        virtual TextureFormat GetFormat() const { return m_Format; }
        virtual uint32_t GetWidth() const { return m_Width; }
        virtual uint32_t GetHeight() const { return m_Height; }
    private:
        RendererID m_RendererID;
        TextureFormat m_Format;
        uint32_t m_Width;
        uint32_t m_Height;
    };
}
