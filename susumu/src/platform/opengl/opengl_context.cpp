#include "supch.h"
#include "platform/opengl/opengl_context.h"

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
        SU_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_WindowHandle);
        int gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        SU_CORE_ASSERT(gladStatus, "Could not initialize Glad!");

        SU_CORE_INFO("OpenGL Info:");
        SU_CORE_INFO("    Vendor: {0}", glGetString(GL_VENDOR));
        SU_CORE_INFO("    Renderer: {0}", glGetString(GL_RENDERER));
        SU_CORE_INFO("    Version: {0}", glGetString(GL_VERSION));

        SU_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Minimum OpenGL version required is 4.5!");
    }

    void OpenGLContext::SwapBuffers()
    {
        SU_PROFILE_FUNCTION();

        glfwSwapBuffers(m_WindowHandle);
    }
}
