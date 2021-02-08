#include "supch.h"
#include "platform/opengl/opengl_framebuffer.h"
#include "engine/renderer/renderer.h"

#include <glad/glad.h>

namespace susumu
{
    OpenGLFramebuffer::OpenGLFramebuffer(uint32_t width, uint32_t height, FramebufferFormat format)
        : m_Width(width), m_Height(height), m_Format(format)
    {
        Resize(width, height);
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        SU_RENDER_S({
            glDeleteFramebuffers(1, &self->m_RendererID);
        });
    }

    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
    {
        if (m_Width == width && m_Height == height) return;

        m_Width = width;
        m_Height = height;

        SU_RENDER_S({
            if (self->m_RendererID)
            {
                glDeleteFramebuffers(1, &self->m_RendererID);
                glDeleteTextures(1, &self->m_ColorAttachment);
                glDeleteTextures(1, &self->m_DepthAttachment);
            }

            glGenFramebuffers(1, &self->m_RendererID);
            glBindFramebuffer(GL_FRAMEBUFFER, self->m_RendererID);

            //color attachment
            glGenTextures(1, &self->m_ColorAttachment);
            glBindTexture(GL_TEXTURE_2D, self->m_ColorAttachment);
            if (self->m_Format == FramebufferFormat::RGBA16F)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, self->m_Width, self->m_Height, 0, GL_RGBA, GL_FLOAT, nullptr);
            }
            else if (self->m_Format == FramebufferFormat::RGBA8)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, self->m_Width, self->m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            }
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, self->m_ColorAttachment, 0);

            //depth attachment
            glGenTextures(1, &self->m_DepthAttachment);
            glBindTexture(GL_TEXTURE_2D, self->m_DepthAttachment);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, self->m_Width, self->m_Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, self->m_DepthAttachment, 0);

            //check status
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                SU_CORE_ERROR("Framebuffer is incomplete!");
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        });
    }

    void OpenGLFramebuffer::Bind() const
    {
        SU_RENDER_S({
            glBindFramebuffer(GL_FRAMEBUFFER, self->m_RendererID);
            glViewport(0, 0, self->m_Width, self->m_Height);
        });
    }

    void OpenGLFramebuffer::Unbind() const
    {
        SU_RENDER_S({
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        });
    }

    void OpenGLFramebuffer::BindTexture(uint32_t slot) const
    {
        SU_RENDER_S1(slot, {
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, self->m_ColorAttachment);
        });
    }
}