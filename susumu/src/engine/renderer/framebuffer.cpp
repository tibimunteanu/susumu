#include "supch.h"
#include "engine/renderer/renderer.h"
#include "engine/renderer/framebuffer.h"
#include "platform/opengl/opengl_framebuffer.h"

namespace susumu
{
    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpec& spec)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None: SU_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLFramebuffer>(spec);

            default: SU_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
        }
    }
}

