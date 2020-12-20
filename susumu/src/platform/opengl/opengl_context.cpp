#include "supch.h"
#include "opengl_context.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace susumu
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
    {
        SU_CORE_ASSERT(windowHandle, "Window handle is null!");
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);
        int gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        SU_CORE_ASSERT(gladStatus, "Could not initialize Glad!");

        SU_CORE_INFO("OpenGL Info:");
        SU_CORE_INFO("    Vendor: {0}", glGetString(GL_VENDOR));
        SU_CORE_INFO("    Renderer: {0}", glGetString(GL_RENDERER));
        SU_CORE_INFO("    Version: {0}", glGetString(GL_VERSION));

    #ifdef SU_ENABLE_ASSERTS
        int versionMajor;
        int versionMinor;
        glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
        glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

        SU_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Minimum OpenGL version required is 4.5!");
    #endif
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
}
