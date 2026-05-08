#pragma once
#include "GT/Renderer/GraphicsContext.h"


struct GLFWwindow;

namespace GT
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		void Init() override;
		void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}


