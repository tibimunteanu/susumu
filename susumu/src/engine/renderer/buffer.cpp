#include "supch.h"
#include "engine/renderer/buffer.h"
#include "platform/opengl/opengl_buffer.h"

namespace susumu
{
    VertexBuffer* VertexBuffer::Create(uint32_t size)
    {
        switch (RendererAPI::Current())
        {
            case RendererAPIType::None: return nullptr;
            case RendererAPIType::OpenGL: return new OpenGLVertexBuffer(size);
            default: return nullptr;
        }
    }

    IndexBuffer* IndexBuffer::Create(uint32_t size)
    {
        switch (RendererAPI::Current())
        {
            case RendererAPIType::None: return nullptr;
            case RendererAPIType::OpenGL: return new OpenGLIndexBuffer(size);
            default: return nullptr;
        }
    }
}

