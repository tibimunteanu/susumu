#include "supch.h"
#include "vertex_array.h"
#include "renderer.h"
#include "platform/opengl/opengl_vertex_array.h"

namespace susumu {

    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None: SU_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexArray>();

            default: SU_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
        }
    }

}
