#include "supch.h"
#include "render_command.h"

#include "platform/opengl/opengl_renderer_api.h"

namespace susumu {

    Scope<RendererAPI> RenderCommand::s_RendererAPI = CreateScope<OpenGLRendererAPI>();

}
