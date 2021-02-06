#include "supch.h"
#include "platform/opengl/opengl_texture.h"
#include "engine/renderer/renderer_api.h"
#include "engine/renderer/renderer.h"

#include <glad/glad.h>

namespace susumu
{
    static GLenum ToOpenGLTextureFormat(TextureFormat format)
    {
        switch (format)
        {
            case TextureFormat::RGB: return GL_RGB;
            case TextureFormat::RGBA: return GL_RGBA;
            default: return 0;
        }
    }

    OpenGLTexture2D::OpenGLTexture2D(TextureFormat format, uint32_t width, uint32_t height)
        : m_Format(format), m_Width(width), m_Height(height)
    {
        auto self = this;
        SU_RENDER_1(self, {
            glGenTextures(1, &self->m_RendererID);
            glBindTexture(GL_TEXTURE_2D, self->m_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, ToOpenGLTextureFormat(self->m_Format), self->m_Width, self->m_Height, 0, ToOpenGLTextureFormat(self->m_Format), GL_UNSIGNED_BYTE, nullptr);
            glBindTexture(GL_TEXTURE_2D, 0);
        });
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        auto self = this;
        SU_RENDER_1(self, {
            glDeleteTextures(1, &self->m_RendererID);
        });
    }
}
