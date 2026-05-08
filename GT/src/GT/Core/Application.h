#pragma once

#include "GT/Core/Base.h"
#include "GT/Events/Event.h"
#include "GT/Events/ApplicationEvent.h"
#include "GT/Events/KeyEvent.h"
#include "GT/Events/MouseEvent.h"
#include "GT/Core/Window.h"
#include "GT/Core/LayerStack.h"
#include "GT/ImGui/ImGuiLayer.h"

#include "GT/Renderer/Shader.h"
#include "GT/Renderer/Buffer.h"
#include "GT/Renderer/VertexArray.h"
#include "GT/Renderer/Renderer.h"
#include "GT/Renderer/OrthographicCamera.h"

#include "GT/Scripting/ScriptEngine.h"

namespace GT {

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			GT_CORE_ASSERT(index < Count,"Index Out Of Range!");
			return Args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string Name = "GT Application";
		std::string WorkingDirectory;
		ApplicationCommandLineArgs CommandLineArgs;
	};



	class HAZEL_API Application
	{
	public:

		Application();
		Application(const ApplicationSpecification& specification);
		virtual ~Application();

		void Init();
		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }	
		ImGuiLayer* GetImGuiLayer() const { return m_ImGuiLayer; }
		inline Window& GetWindow() { return *m_Window; }
		void Close() { m_Running = false; }


		const ApplicationSpecification& GetSpecification() const { return m_Specification; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;


		ApplicationSpecification m_Specification;

		Timestep m_Timestep;
		float m_LastFrameTime = 0.0f;
		static Application* s_Instance;

		bool m_Minimized = false;
	};

	// to be defined in client
	Application* CreateApplication(ApplicationCommandLineArgs args);

}
