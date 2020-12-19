#include "supch.h"
#include "opengl_context.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace susumu {

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
    }

    void OpenGLContext::SwapBuffers() 
    {
        glfwSwapBuffers(m_WindowHandle);
    }

}
