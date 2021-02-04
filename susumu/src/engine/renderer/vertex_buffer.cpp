#include "supch.h"
#include "engine/renderer/vertex_buffer.h"
#include "platform/opengl/opengl_vertex_buffer.h"

namespace susumu
{
    VertexBuffer* VertexBuffer::Create(unsigned int size)
    {
        switch (RendererAPI::Current())
        {
            case RendererAPIType::None: return nullptr;
            case RendererAPIType::OpenGL: return new OpenGLVertexBuffer(size);
            default: return nullptr;
        }
    }
}

