#pragma once

#include "engine/renderer/graphics_context.h"

struct GLFWwindow;

namespace susumu {

    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(GLFWwindow* windowHandle);
        virtual ~OpenGLContext();
        virtual void Init() override;
        virtual void SwapBuffers() override;
    private:
        GLFWwindow* m_WindowHandle;
    };

}
