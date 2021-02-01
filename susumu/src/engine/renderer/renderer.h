#pragma once

#include "engine/renderer/render_command_queue.h"
#include "engine/renderer/renderer_api.h"

namespace susumu
{
    class Renderer
    {
    public:
        static void Clear();
        static void Clear(float r, float g, float b, float a = 1.0f);
        static void SetClearColor(float r, float g, float b, float a);

        static void ClearMagenta();

        static void Init();

        static void Submit(const std::function<void()>& command)
        {
            //s_Instance->m_CommandQueue.Submit(command);
        }

        void WaitAndRender();

        inline static Renderer& Get() { return *s_Instance; }
    private:
        static Renderer* s_Instance;

        RenderCommandQueue m_CommandQueue;
    };

#define SU_RENDER(x) ::susumu::Renderer::Submit([=](){ RendererAPI::DrawIndexed; })
}
