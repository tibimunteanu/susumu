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

    OpenGLContext::~OpenGLContext()
    {
    }

    void OpenGLContext::Init() 
    {
        glfwMakeContextCurrent(m_WindowHandle);
        int gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        SU_CORE_ASSERT(gladStatus, "Could not initialize Glad!");
    }

    void OpenGLContext::SwapBuffers() 
    {
        glfwSwapBuffers(m_WindowHandle);
    }

}
