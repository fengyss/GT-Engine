#include "gtpch.h"
#include "ImGuiLayer.h"




#include "imgui.h"
#include "ImGuizmo.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


#include "GT/Core/Application.h"

//TEMPORARY
#include "Glad/glad.h"
#include "GLFW/glfw3.h"

namespace GT
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

    void ImGuiLayer::OnAttach()
    {
        GT_PROFILE_FUNCTION();
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        //io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // We can honor GetMouseCursor() values (optional)
        //io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;  // We can honor io.WantSetMousePos requests (optional, rarely used)

        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        //io.ConfigFlags |= ImGuiViewportFlags_NoInputs;
        //set up Dear ImGui style
		//ImGui::StyleColorsClassic();

		io.FontDefault = io.Fonts->AddFontFromFileTTF("project/assets/fonts/opensans/OpenSans-Regular.ttf", 18.0f);
        io.Fonts->AddFontFromFileTTF("project/assets/fonts/opensans/OpenSans-Bold.ttf", 18.0f);


        //ImGui::StyleColorsClassic();
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        //when viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        SetDarkThemeColors();

	    //Fix: Correctly cast the window object to GLFWwindow*
		auto& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
        
		//set up Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

	void ImGuiLayer::OnDetach()
	{
        GT_PROFILE_FUNCTION();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}



    void ImGuiLayer::Begin()
    {
        GT_PROFILE_FUNCTION();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
	}

    void ImGuiLayer::End()
    {
        GT_PROFILE_FUNCTION();
        ImGuiIO& io = ImGui::GetIO();

        Application& app = Application::Get();

        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        // Rendering
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();

            ImGui::UpdatePlatformWindows();

            ImGui::RenderPlatformWindowsDefault();

            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::SetDarkThemeColors()
    {
        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    }
  
    void ImGuiLayer::OnImGuiRender()
    {
        //static bool show_demo_window = true;
        //ImGui::ShowDemoWindow(&show_demo_window);

    }

    void ImGuiLayer::OnEvent(Event& event)
    {
        if (m_Block)
        {
            ImGuiIO& io = ImGui::GetIO();
            event.Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
            event.Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
        }
    }


 
	void ImGuiLayer::OnUpdate(Timestep ts)
	{

	}
    /*
   
        void ImGuiLayer::OnEvent(Event & event)
        {
            EventDispatcher dispatcher(event);
            dispatcher.Dispatch<MouseButtonPressedEvent>(GT_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
            dispatcher.Dispatch<MouseButtonReleasedEvent>(GT_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));

            dispatcher.Dispatch<MouseMovedEvent>(GT_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
            dispatcher.Dispatch<MouseScrolledEvent>(GT_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));

            dispatcher.Dispatch<KeyPressedEvent>(GT_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
            dispatcher.Dispatch<KeyReleasedEvent>(GT_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
            dispatcher.Dispatch<KeyTypedEvent>(GT_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));

            dispatcher.Dispatch<WindowResizeEvent>(GT_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
        }

        bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent & e)
        {
            ImGuiIO& io = ImGui::GetIO();
            //io.MouseDown[e.GetMouseButton()] = true;
            io.AddMouseButtonEvent(e.GetMouseButton(), true);

            return false;
        }

        bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent & e)
        {
            ImGuiIO& io = ImGui::GetIO();
            //io.MouseDown[e.GetMouseButton()] = false;
            io.AddMouseButtonEvent(e.GetMouseButton(), false);

            return false;
        }

        bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent & e)
        {
            ImGuiIO& io = ImGui::GetIO();
            //io.MousePos = ImVec2(e.GetX(), e.GetY());
            io.AddMousePosEvent(e.GetX(), e.GetY());

            return false;
        }

        bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent & e)
        {
            ImGuiIO& io = ImGui::GetIO();
            //io.MouseWheelH += e.GetXOffset();
            //io.MouseWheel += e.GetYOffset();
            io.AddMouseWheelEvent(e.GetXOffset(), e.GetYOffset());

            return false;
        }

        bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent & e)
        {
            ImGuiIO& io = ImGui::GetIO();
            //io.KeysDown[e.GetKeyCode()] = true;   // This line is deprecated in newer ImGui versions
            // Cast the integer key code to ImGuiKey to resolve the type mismatch
            io.AddKeyEvent(Glfw_KeyToImGuiKey(e.GetKeyCode()), true);

            io.KeyCtrl = ImGui::IsKeyPressed(ImGuiKey_LeftCtrl) || ImGui::IsKeyPressed(ImGuiKey_RightCtrl);
            io.KeyShift = ImGui::IsKeyPressed(ImGuiKey_LeftShift) || ImGui::IsKeyPressed(ImGuiKey_RightShift);
            io.KeyAlt = ImGui::IsKeyPressed(ImGuiKey_LeftAlt) || ImGui::IsKeyPressed(ImGuiKey_RightAlt);
            io.KeySuper = ImGui::IsKeyPressed(ImGuiKey_LeftSuper) || ImGui::IsKeyPressed(ImGuiKey_RightSuper); //windows key in windows, command key in macOS

            return false;
        }

        bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent & e)
        {
            ImGuiIO& io = ImGui::GetIO();
            //io.KeysDown[e.GetKeyCode()] = false; // This line is deprecated in newer ImGui versions
            // 
            // Cast the integer key code to ImGuiKey to resolve the type mismatch
            io.AddKeyEvent(Glfw_KeyToImGuiKey(e.GetKeyCode()), false);

            io.KeyCtrl = ImGui::IsKeyReleased(ImGuiKey_LeftCtrl) || ImGui::IsKeyReleased(ImGuiKey_RightCtrl);
            io.KeyShift = ImGui::IsKeyReleased(ImGuiKey_LeftShift) || ImGui::IsKeyReleased(ImGuiKey_RightShift);
            io.KeyAlt = ImGui::IsKeyReleased(ImGuiKey_LeftAlt) || ImGui::IsKeyReleased(ImGuiKey_RightAlt);
            io.KeySuper = ImGui::IsKeyReleased(ImGuiKey_LeftSuper) || ImGui::IsKeyReleased(ImGuiKey_RightSuper); //windows key in windows, command key in macOS


            return false;
        }

        bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent & e)
        {
            ImGuiIO& io = ImGui::GetIO();
            int keycode = e.GetKeyCode();
            if (keycode > 0 && keycode < 0x10000)
                io.AddInputCharacter(keycode);

            return false;
        }

        bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent & e)
        {
            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = ImVec2((float)e.GetWidth(), (float)e.GetHeight());
            io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

            glViewport(0, 0, e.GetWidth(), e.GetHeight());

            return false;
        }
    */
}