#pragma once

namespace susumu
{
    class RendererAPI
    {
    public:
        static void Clear(float r, float g, float b, float a);
        static void SetClearColor(float r, float g, float b, float a);
    };
}
