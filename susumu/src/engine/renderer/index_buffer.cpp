#include "supch.h"
#include "engine/renderer/index_buffer.h"
#include "platform/opengl/opengl_index_buffer.h"

namespace susumu
{
    IndexBuffer* IndexBuffer::Create(unsigned int size)
    {
        switch (RendererAPI::Current())
        {
            case RendererAPIType::None: return nullptr;
            case RendererAPIType::OpenGL: return new OpenGLIndexBuffer(size);
            default: return nullptr;
        }
    }
}
