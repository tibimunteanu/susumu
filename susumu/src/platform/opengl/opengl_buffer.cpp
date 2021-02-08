#include "supch.h"
#include "platform/opengl/opengl_buffer.h"

#include <glad/glad.h>

namespace susumu
{
    //vertex buffer
    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
        : m_RendererID(0), m_Size(size)
    {
        SU_RENDER_S({
            glGenBuffers(1, &self->m_RendererID);
        });
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        SU_RENDER_S({
            glDeleteBuffers(1, &self->m_RendererID);
        });
    }

    void OpenGLVertexBuffer::SetData(void* buffer, uint32_t size, uint32_t offset)
    {
        SU_RENDER_S3(buffer, size, offset, {
            glBindBuffer(GL_ARRAY_BUFFER, self->m_RendererID);
            glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
        });
    }

    void OpenGLVertexBuffer::Bind() const
    {
        SU_RENDER_S({
            glBindBuffer(GL_ARRAY_BUFFER, self->m_RendererID);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (const void*)(3 * sizeof(float)));
        });
    }

    //index buffer
    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t size)
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

    void OpenGLIndexBuffer::SetData(void* buffer, uint32_t size, uint32_t offset)
    {
        m_Size = size;
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
