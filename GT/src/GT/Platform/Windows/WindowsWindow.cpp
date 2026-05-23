#include "gtpch.h"
#include "WindowsWindow.h"

#include "GT/Events/ApplicationEvent.h"
#include "GT/Events/KeyEvent.h"
#include "GT/Events/MouseEvent.h"

#include "GT/Platform/OpenGL/OpenGLContext.h"
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <windows.h>

namespace GT
{
	static bool s_GLFWWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		GT_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		GT_PROFILE_FUNCTION();
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	

	void WindowsWindow::Init(const WindowProps& props)
	{
		GT_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		GT_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		int major, minor, rev;
		glfwGetVersion(&major, &minor, &rev);
		GT_CORE_INFO("GLFW version {0}, {1}, {2}", major, minor, rev);

		if (!s_GLFWWInitialized)
		{
			//TODO: glfwTerminate on system shutdown
			bool success;
			{
				GT_PROFILE_SCOPE("glfwCreateWindow");
				success = glfwInit();
			}
			GT_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWWInitialized = true;
		}
		{
			glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
			//glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
			//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
			//glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

			//glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);  // 避免窗口获得焦点
			//glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);  // 主方案

		}

		{
			GT_PROFILE_SCOPE("glfwCreateWindow");
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		}


		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();





		glfwSetWindowUserPointer(m_Window, &m_Data);

		SetVSync(m_Data.VSync);


		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			if (focused)
			{
				WindowFocusEvent event;
				data.EventCallback(event);
			}
			else
			{
				WindowLostFocusEvent event;
				data.EventCallback(event);
			}
		});

		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowMovedEvent event(xpos,ypos);
			data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, true);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int key)
		{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent event(key);
				data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});

	}

	void WindowsWindow::Shutdown()
	{
		GT_PROFILE_FUNCTION();
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		GT_PROFILE_FUNCTION();
		glfwPollEvents();
	}

	void WindowsWindow::OnRender()
	{
		GT_PROFILE_FUNCTION();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::GetPosition(int& xpos, int& ypos) const
	{
		glfwGetWindowPos(m_Window, &xpos, &ypos);
	}
	void WindowsWindow::SetCursorPosition(int xpos, int ypos) const
	{
		glfwSetCursorPos(m_Window, xpos, ypos);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::SetCursorClip(int type) const
	{
		HWND hwnd = glfwGetWin32Window(m_Window);

		RECT clipRect;
		GetClientRect(hwnd, &clipRect); // 获取窗口客户区（不包含边框和标题栏）
		MapWindowPoints(hwnd, nullptr, (LPPOINT)&clipRect, 2); // 转换为屏幕坐标

		switch (type)
		{
		case 1:
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			break;
		case 2:
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			break;
		};
		
	}

	void WindowsWindow::SetWindowTransparentAndMousePassthrough(bool open)
	{
		if (open)
		{
			glfwSetWindowAttrib(m_Window, GLFW_DECORATED, GLFW_FALSE);
			glfwSetWindowAttrib(m_Window, GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);

		}
		else
		{
			glfwSetWindowAttrib(m_Window, GLFW_DECORATED, GLFW_TRUE);
			glfwSetWindowAttrib(m_Window, GLFW_MOUSE_PASSTHROUGH, GLFW_FALSE);
		}
	}


}