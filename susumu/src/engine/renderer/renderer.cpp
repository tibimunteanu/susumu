#include "supch.h"
#include "engine/renderer/renderer.h"

namespace susumu
{
    Renderer* Renderer::s_Instance = new Renderer();

    void Renderer::Init()
    {
    }

    void Renderer::Clear()
    {
    }

    void Renderer::Clear(float r, float g, float b, float a)
    {
        SU_RENDER_4(r, g, b, a, {
            RendererAPI::Clear(r, g, b, a);
        });
    }

    void Renderer::ClearMagenta()
    {
        Clear(1, 0, 1);
    }

    void Renderer::SetClearColor(float r, float g, float b, float a)
    {
    }

    void Renderer::WaitAndRender()
    {
        s_Instance->m_CommandQueue.Execute();
    }
}
