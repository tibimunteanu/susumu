#include "supch.h"
#include "render_command.h"

#include "platform/opengl/opengl_renderer_api.h"

namespace susumu {

    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}
