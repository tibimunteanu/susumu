#include "supch.h"
#include "platform/opengl/opengl_index_buffer.h"

#include <glad/glad.h>

namespace susumu
{
    OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int size)
        : m_RendererID(0), m_Size(size)
    {
        SU_RENDER_S({
            glGenBuffers(1, &self->m_RendererID);
        });
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        SU_RENDER_S({
            glDeleteBuffers(1, &self->m_RendererID);
        });
    }

    void OpenGLIndexBuffer::SetData(void* buffer, unsigned int size, unsigned int offset)
    {
        SU_RENDER_S3(buffer, size, offset, {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_RendererID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
        });
    }

    void OpenGLIndexBuffer::Bind() const
    {
        SU_RENDER_S({
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_RendererID);
        });
    }
}
