#include "gtpch.h"
#include "Application.h"

#include "GT/Assets/AssetsManager.h"
#include "Input.h"

#include "imgui.h"

#include "GT/Utils/PlatformUtils.h"
#include "GT/Math/Random.h"

#define EnableImGui


namespace GT
{

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)




	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		GT_PROFILE_FUNCTION();

		GT_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		

#ifdef EnableImGui 
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
#endif
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		Init();

		GT_CORE_INFO("Application Created!");
	}

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		GT_PROFILE_FUNCTION();

		GT_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// Set working directory here
		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_Specification.WorkingDirectory);

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(m_Specification.Name)));
		m_Window->SetEventCallback(GT_BIND_EVENT_FN(Application::OnEvent));


		Init();

#ifdef EnableImGui 
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
#endif

	}


	Application::~Application()
	{
	}


	void Application::Init()
	{
		AssetsManager::Init();
		Renderer::Init();
		//ScriptEngine::Init();
		Random::Init();
	}

	void Application::PushLayer(Layer* layer)
	{
		GT_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		GT_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		GT_PROFILE_FUNCTION();

		//GT_CORE_INFO("Event: {0}", e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
		//GT_CORE_ERROR("{0}", e);

		for (auto it = m_LayerStack.end();it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}

	}

	void Application::Run()
	{

		GT_PROFILE_FUNCTION();

		while (m_Running) {
			GT_PROFILE_SCOPE("Run Loop");
			float time = Time::GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if(Input::IsKeyPressed(GT_KEY_ESCAPE))
				m_Running = false;

			m_Window->OnUpdate();

			if (!m_Minimized)
			{
				GT_PROFILE_SCOPE("Layerstack OnUpdate");
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}


#ifdef EnableImGui 
			m_ImGuiLayer->Begin();
			{
				GT_PROFILE_SCOPE("Layerstack OnImGuiRender");
				for (Layer* layer : m_LayerStack) {
					layer->OnImGuiRender();
				}
			}
			m_ImGuiLayer->End();
#endif


			m_Window->OnRender();
		}
		Renderer2D::Shutdown();
	}


	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		GT_CORE_INFO(e);
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		GT_PROFILE_FUNCTION();
		GT_CORE_INFO(e);
		if (e.GetHeight() == 0 || e.GetWidth() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}

}