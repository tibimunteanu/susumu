#include "supch.h"
#include "buffer.h"
#include "renderer.h"
#include "platform/opengl/opengl_buffer.h"

namespace susumu {

    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None: SU_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertices, size);

            default: SU_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
        }
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None: SU_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(indices, count);

            default: SU_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
        }
    }

}
