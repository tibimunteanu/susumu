#include "supch.h"
#include "engine/renderer/renderer.h"

namespace susumu
{
    Renderer* Renderer::s_Instance = new Renderer();
    RendererAPIType RendererAPI::s_CurrentRendererAPI = RendererAPIType::OpenGL;

    void Renderer::Init()
    {
        s_Instance->m_ShaderLibrary = std::make_unique<ShaderLibrary>();
        SU_RENDER({ RendererAPI::Init(); });

		Renderer::GetShaderLibrary()->Load("assets/shaders/susumuPBR_Static.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/susumuPBR_Anim.glsl");
    }

    void Renderer::Clear()
    {
        SU_RENDER({
            RendererAPI::Clear(0.0f, 0.0f, 0.0f, 1.0f);
        });
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

    void Renderer::DrawIndexed(uint32_t count, bool depthTest)
    {
        SU_RENDER_2(count, depthTest, {
            RendererAPI::DrawIndexed(count, depthTest);
        });
    }

    void Renderer::WaitAndRender()
    {
        s_Instance->m_CommandQueue.Execute();
    }
}
