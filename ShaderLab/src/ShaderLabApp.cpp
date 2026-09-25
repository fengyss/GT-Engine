#include "gtpch.h"


#include <GT.h>

#include "imgui.h"

#define BIND_EVENT_FN(x) std::bind(&ShaderLab::x, this, std::placeholders::_1)

static char shader[1024 * 30];
glm::vec4 color = { 0.0f,0.0f,0.0f,0.0f };
bool Enable[32];
class ShaderLab
{
public:
	ShaderLab(const std::filesystem::path& file)
	{
		Init();


		// add watch file for shader
		m_FileWatcher = GT::CreateScope<GT::FileWatcher>();
		m_FileWatcher->Watch(file, WhenFileChanged);
		VertexShader =
"#version 460 core						\n\
layout(location = 0) in vec3 a_Position;\n\
void main()								\n\
{										\n\
	gl_Position = vec4(a_Position,1.0f);\n\
}";
		FragmentShader =
"#version 460 core						\n\
layout(location = 0) out vec4 o_Color;	\n\
uniform vec2 u_Resolution;				\n\
uniform vec2 u_Mouse;					\n\
uniform float u_Time;					\n\
uniform sampler2D u_Textures[32];		\n\
void main()								\n\
{										\n\
    vec2 st = gl_FragCoord.xy/u_Resolution; \n\
	st*=3.0f;					\n\
	int i = int(floor(st.x)+floor(st.y));					\n\
	o_Color	= texture(u_Textures[i],fract(st));	\n\
}";

		memset(shader, 0, sizeof(shader));
		strncpy(shader, FragmentShader.c_str(), sizeof(shader));

		CompileShader();

		m_VertexArray = GT::VertexArray::Create();

		m_VertexBuffer = GT::VertexBuffer::Create(3 * 4 * sizeof(float));
		{
			GT::BufferLayout layout = {
				{ GT::ShaderDataType::Float3, "a_Position"  },
			};
			m_VertexBuffer->SetLayout(layout);
		}
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		glm::vec3* VertexPositions = new glm::vec3[4];
		VertexPositions[0] = { -1.0f, -1.0f, 0.0f };
		VertexPositions[1] = { 1.0f, -1.0f, 0.0f };
		VertexPositions[2] = { 1.0f, 1.0f, 0.0f };
		VertexPositions[3] = { -1.0f, 1.0f, 0.0f };

		m_VertexBuffer->SetData(VertexPositions, 3 * 4 * 4);


		uint32_t* quadIndices = new uint32_t[6];

		quadIndices[0] = 0;
		quadIndices[1] = 1;
		quadIndices[2] = 2;

		quadIndices[3] = 2;
		quadIndices[4] = 3;
		quadIndices[5] = 0;

		GT::Ref<GT::IndexBuffer> squareIB;
		squareIB = GT::IndexBuffer::Create((float*)quadIndices, 6);
		m_VertexArray->SetIndexBuffer(squareIB);
		delete[] quadIndices;

		for (int i = 0;i < 32;i++)Enable[i] = true;
	}
	void Init()
	{
		App = new GT::Application("ShaderLab");
		m_Resolution = { 1600,900 };
		App->GetWindow().SetEventCallback(GT_BIND_EVENT_FN(ShaderLab::OnEvent));


		m_ImGuiLayer = new GT::ImGuiLayer();
		m_ImGuiLayer->OnAttach();

		GT::RenderCommand::Init();

	}
	void OnEvent(GT::Event& e)
	{

		GT::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<GT::WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<GT::WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
		dispatcher.Dispatch<GT::KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressed));
		dispatcher.Dispatch<GT::MouseButtonPressedEvent>(BIND_EVENT_FN(OnMouseButtonPressed));
		//GT_CORE_ERROR("{0}", e);
	}

	bool OnKeyPressed(GT::KeyPressedEvent& event)
	{
		if (event.GetKeyCode() == GT::Key::Escape)
		{
			m_Running = false;
		}
		if (event.GetKeyCode() == GT::Key::P)
		{
			EnableMenuBar ^= true;
		}
		return true;

	}
	bool OnMouseButtonPressed(GT::MouseButtonPressedEvent& event)
	{
		return true;
	}

	bool OnWindowClose(GT::WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool OnWindowResize(GT::WindowResizeEvent& e)
	{
		GT::RenderCommand::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
		m_Resolution = { e.GetWidth(), e.GetHeight() };
		return true;
	}
	void CompileShader()
	{
		FragmentShader = shader;

		GT::Ref<GT::ShaderAsset> newshader = GT::ShaderAsset::Create("ShaderLab", VertexShader, FragmentShader);

		if (newshader)
		{
			m_Shader.reset();
			m_Shader = newshader;
		}

		int32_t samplers[32];
		for (uint32_t i = 0;i < 32;i++)
		{
			samplers[i] = i;
		}

		m_Shader->Bind();
		m_Shader->SetUniformiv("u_Textures", samplers, 32);
	}
	void OnUpdate(GT::Timestep ts)
	{
		App->GetWindow().OnUpdate();

		m_Time += ts;

		auto mouse = GT::Input::GetMousePosition();
		if (mouse.first<0 || mouse.first>m_Resolution.x || mouse.second<0 || mouse.second>m_Resolution.y)
			return;
		m_Mouse = { mouse.first, mouse.second };


	}
	void OnRender()
	{

		GT::RenderCommand::Clear();
		m_Shader->Bind();
		m_Shader->SetUniform2f("u_Mouse", glm::vec2(m_Mouse.x, m_Mouse.y));
		m_Shader->SetUniform1f("u_Time", m_Time);
		m_Shader->SetUniform2f("u_Resolution", m_Resolution);
		GT::RenderCommand::DrawIndexed(m_VertexArray, 6);

		OnImguiRender();

		App->GetWindow().OnRender();

	}
	void OnImguiRender()
	{
		m_ImGuiLayer->Begin();

		ImGui::Begin("Shader");

		ImVec2 size = ImGui::GetContentRegionAvail();
		size.y -= ImGui::GetStyle().FramePadding.y * 2;

		static float lastcompile = 0.0f;
		bool Modified = ImGui::InputTextMultiline("##Shader", shader, sizeof(shader), size);
		static bool Recompile = false;
		if (Modified)
		{
			Recompile = true;
			lastcompile = GT::Time::GetTime();
		}
		if (Recompile)
		{
			float current = GT::Time::GetTime();
			float duration = current - lastcompile;

			if (duration >= 1)
			{
				Recompile = false;
				lastcompile = current;
				CompileShader();
			}
		}
		ImGui::End();


		ImGui::Begin("Property");
		ImGui::Text("%s : %.f %.f", "Mouse", m_Mouse.x, m_Mouse.y);
		ImGui::Text("%s : %.f %.f", "Resolution", m_Resolution.x, m_Resolution.y);
		if (ImGui::ColorEdit4("Background Color", (float*)&color))
		{
			GT::RenderCommand::SetClearColor(color);
		}
		bool open = ImGui::TreeNode("Textures");
		if(open)
		{
			for (int i = 0;i < 32;i++)
			{
				std::string name = "Texture" + std::to_string(i);
				if (ImGui::Button(name.c_str()))
				{
					std::filesystem::path path = GT::FileDialogs::OpenTextureFile();
					m_Textures[i].reset();
					m_Textures[i] = GT::Texture2DAsset::Create(path);
				}

				name = "Enable##" + std::to_string(i);
				ImGui::SameLine();
				ImGui::Checkbox(name.c_str(), &Enable[i]);

				if (m_Textures[i] == nullptr) continue;

				if (Enable[i])
					m_Textures[i]->Bind(i);
				else
					m_Textures[i]->Unbind();


				ImGui::SameLine();
				ImGui::Text("%s", m_Textures[i]->GetName().c_str());
				ImGui::Image(m_Textures[i]->GetRendererID(), ImVec2(40, 40), { 0, 1 }, { 1, 0 });
			}
			ImGui::TreePop();
		}

		ImGui::End();

		if(EnableMenuBar)
			OnMenuBarRender();

		m_ImGuiLayer->End();
	}
	void OnMenuBarRender()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open file", "Ctrl+O"))
					;


				if (ImGui::MenuItem("Exit"))
					m_Running = false;

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Setting"))
			{
				if (ImGui::MenuItem("Unreal Theme", ""))
				{
					m_ImGuiLayer->SetTheme(GT::ImGuiTheme::Unreal);
				}
				if (ImGui::MenuItem("Vscode Theme", ""))
				{
					m_ImGuiLayer->SetTheme(GT::ImGuiTheme::VSCode);
				}
				if (ImGui::MenuItem("Dark Theme", ""))
				{
					m_ImGuiLayer->SetTheme(GT::ImGuiTheme::Dark);
				}
				if (ImGui::MenuItem("SoftLight Theme", ""))
				{
					m_ImGuiLayer->SetTheme(GT::ImGuiTheme::SoftLight);
				}
				if (ImGui::MenuItem("Cyberpunk Theme", ""))
				{
					m_ImGuiLayer->SetTheme(GT::ImGuiTheme::Cyberpunk);
				}
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}
	static void WhenFileChanged(const std::filesystem::path& path, GT::FileAction action)
	{
		switch (action)
		{
		case GT::FileAction::Added:
			GT_CORE_INFO("File added: {0}", path.string());
			break;

		case GT::FileAction::Removed:
			GT_CORE_WARN("File removed: {0}", path.string());
			break;

		case GT::FileAction::Modified:
			GT_CORE_INFO("File modified: {0}", path.string());
			break;

		case GT::FileAction::Renamed:
			GT_CORE_TRACE("File renamed: {0}", path.string());
			break;
		}

	}

	bool IsRunning() { return m_Running; }
private:

	GT::ImGuiLayer* m_ImGuiLayer;

	GT::Application* App;

	GT::Scope<GT::FileWatcher> m_FileWatcher;
	std::string VertexShader, FragmentShader;
	GT::Ref<GT::ShaderAsset> m_Shader;
	GT::Ref<GT::VertexArray> m_VertexArray;
	GT::Ref<GT::VertexBuffer> m_VertexBuffer;
	GT::Ref<GT::TextureAsset> m_Textures[32];
	float m_Time = 0.0f;

	glm::vec2 m_Mouse = { 0,0 }, m_Resolution = { 1600,900 };

	bool m_Running = true;
	bool EnableMenuBar = false;
};

int main(int argc, char** argv)
{
	GT::Log::Init();

	ShaderLab lab("");
	float m_LastFrameTime = GT::Time::GetTime();
	
	while (lab.IsRunning())
	{
		float time = GT::Time::GetTime();
		GT::Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		lab.OnUpdate(timestep);

		lab.OnRender();
	}

	GT::Log::ShutDown();
}