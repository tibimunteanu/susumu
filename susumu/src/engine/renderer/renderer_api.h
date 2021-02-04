#pragma once

namespace susumu
{
    using RendererID = unsigned int;

    enum class RendererAPIType
    {
        None, 
        OpenGL
    };

    class RendererAPI
    {
    public:
        static void Init();
        static void Shutdown();
        static void Clear(float r, float g, float b, float a);
        static void SetClearColor(float r, float g, float b, float a);
        static void DrawIndexed(unsigned int count);

        static RendererAPIType Current() { return s_CurrentRendererAPI; }
    private:
        static RendererAPIType s_CurrentRendererAPI;
    };
}
