#pragma once

#include "GT/Core/Window.h"
#include "GT/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace GT {
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override; 
		void OnRender() override;

		virtual inline unsigned int GetWidth() const override { return m_Data.Width; }
		virtual inline unsigned int GetHeight() const override { return m_Data.Height; }


		virtual void GetPosition(int& xpos, int& ypos) const override;
		virtual void SetCursorPosition(int xpos, int ypos) const override;

		// Window attributes
		virtual inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		virtual void SetCursorClip(int type) const override;

		virtual void SetWindowTransparentAndMousePassthrough(bool open) override;

		inline void* GetNativeWindow() const override { return m_Window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync = true;
			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}

