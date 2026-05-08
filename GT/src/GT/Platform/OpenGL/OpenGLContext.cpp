#include "gtpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace GT
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
		GT_CORE_ASSERT(windowHandle, "Window handle is null!");
	}
	void OpenGLContext::Init()
	{
		GT_PROFILE_FUNCTION();
		glfwMakeContextCurrent(m_WindowHandle);  // setted  glViewport the first time here
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GT_CORE_ASSERT(status, "Failed to initialize Glad!");
		
		GT_CORE_INFO("OpenGL Info :");
		GT_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		GT_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		GT_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
	}
	void OpenGLContext::SwapBuffers()
	{
		GT_PROFILE_FUNCTION();
		glfwSwapBuffers(m_WindowHandle);
	}
}