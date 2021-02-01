#include "supch.h"
#include "engine/renderer/renderer_api.h"

#include <glad/glad.h>

namespace susumu
{
    void RendererAPI::SetClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }

    void RendererAPI::Clear(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}
