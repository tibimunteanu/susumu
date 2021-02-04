#pragma once

#include "engine/renderer/vertex_buffer.h"

namespace susumu
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
        public:
            OpenGLVertexBuffer(unsigned int size);
            virtual ~OpenGLVertexBuffer();

            virtual void SetData(void* buffer, unsigned int size, unsigned int offset = 0);
            virtual void Bind() const;

            virtual unsigned int GetSize() const { return m_Size; }
            virtual RendererID GetRendererID() const { return m_RendererID; }
    private:
        RendererID m_RendererID;
        unsigned int m_Size;
    };
}
