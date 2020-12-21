#include "supch.h"
#include "engine/renderer/render_command.h"

namespace susumu
{
    Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}
