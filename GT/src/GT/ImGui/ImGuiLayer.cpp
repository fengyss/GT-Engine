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

		io.FontDefault = io.Fonts->AddFontFromFileTTF("Resources/fonts/opensans/OpenSans-Regular.ttf", 18.0f);
        io.Fonts->AddFontFromFileTTF("Resources/fonts/opensans/OpenSans-Bold.ttf", 18.0f);



        //when viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ApplyUnrealTheme();

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
    void ImGuiLayer::SetTheme(ImGuiTheme theme)
    {
        switch (theme)
        {
        case ImGuiTheme::Cyberpunk:
            ApplyCyberpunkTheme();
            break;
        case ImGuiTheme::Unreal:
            ApplyUnrealTheme();
            break;
        case ImGuiTheme::VSCode:
            ApplyVSCodeTheme();
            break;
		case ImGuiTheme::SoftLight:
			ApplySoftLightTheme();
			break;
        case ImGuiTheme::Dark:
			ImGui::StyleColorsDark();
			break;
        default:
            break;
        }
    }
    void ImGuiLayer::ApplyVSCodeTheme()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        style.WindowPadding = ImVec2(8, 8);
        style.FramePadding = ImVec2(6, 4);
        style.ItemSpacing = ImVec2(8, 6);
        style.WindowRounding = 4.0f;
        style.FrameRounding = 4.0f;
        style.TabRounding = 4.0f;

        ImVec4 bg = ImVec4(0.13f, 0.15f, 0.19f, 1.00f); // #21252b
        ImVec4 sidebar = ImVec4(0.17f, 0.19f, 0.23f, 1.00f); // #282c34
        ImVec4 active = ImVec4(0.26f, 0.30f, 0.37f, 1.00f);
        ImVec4 blue = ImVec4(0.33f, 0.58f, 0.91f, 1.00f); // #528bff
        ImVec4 purple = ImVec4(0.68f, 0.51f, 0.93f, 1.00f);
        ImVec4 text = ImVec4(0.86f, 0.88f, 0.91f, 1.00f);

        colors[ImGuiCol_WindowBg] = bg;
        colors[ImGuiCol_ChildBg] = sidebar;
        colors[ImGuiCol_PopupBg] = sidebar;

        colors[ImGuiCol_Text] = text;
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.53f, 0.60f, 1.00f);

        colors[ImGuiCol_FrameBg] = sidebar;
        colors[ImGuiCol_FrameBgHovered] = active;
        colors[ImGuiCol_FrameBgActive] = ImVec4(blue.x, blue.y, blue.z, 0.3f);

        colors[ImGuiCol_Button] = sidebar;
        colors[ImGuiCol_ButtonHovered] = active;
        colors[ImGuiCol_ButtonActive] = ImVec4(blue.x, blue.y, blue.z, 0.5f);

        colors[ImGuiCol_Header] = sidebar;
        colors[ImGuiCol_HeaderHovered] = active;
        colors[ImGuiCol_HeaderActive] = ImVec4(blue.x, blue.y, blue.z, 0.4f);

        colors[ImGuiCol_Tab] = bg;
        colors[ImGuiCol_TabActive] = blue;
        colors[ImGuiCol_TabHovered] = active;

        colors[ImGuiCol_Border] = ImVec4(0.22f, 0.24f, 0.28f, 1.00f);
        colors[ImGuiCol_SliderGrab] = blue;
        colors[ImGuiCol_SliderGrabActive] = purple;
        colors[ImGuiCol_CheckMark] = blue;

        colors[ImGuiCol_ScrollbarBg] = sidebar;
        colors[ImGuiCol_ScrollbarGrab] = active;
        colors[ImGuiCol_ScrollbarGrabHovered] =  ImVec4(blue.x, blue.y, blue.z, 0.7f);
        colors[ImGuiCol_ScrollbarGrabActive] = blue;
    }
    void ImGuiLayer::ApplyUnrealTheme()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        style.WindowPadding = ImVec2(8, 8);
        style.FramePadding = ImVec2(6, 4);
        style.ItemSpacing = ImVec2(8, 6);
        style.WindowRounding = 2.0f;
        style.FrameRounding = 2.0f;
        style.GrabRounding = 2.0f;
        style.TabRounding = 2.0f;

        ImVec4 bg0 = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
        ImVec4 bg1 = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        ImVec4 bg2 = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
        ImVec4 orange = ImVec4(0.95f, 0.55f, 0.15f, 1.00f);
        ImVec4 text = ImVec4(0.88f, 0.88f, 0.88f, 1.00f);

        colors[ImGuiCol_WindowBg] = bg0;
        colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
        colors[ImGuiCol_PopupBg] = bg1;

        colors[ImGuiCol_Text] = text;
        colors[ImGuiCol_TextDisabled] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);

        colors[ImGuiCol_FrameBg] = bg1;
        colors[ImGuiCol_FrameBgHovered] = bg2;
        colors[ImGuiCol_FrameBgActive] =  ImVec4(orange.x, orange.y, orange.z, 0.3f);

        colors[ImGuiCol_Button] = bg1;
        colors[ImGuiCol_ButtonHovered] = bg2;
        colors[ImGuiCol_ButtonActive] = orange;

        colors[ImGuiCol_Header] = bg1;
        colors[ImGuiCol_HeaderHovered] = bg2;
        colors[ImGuiCol_HeaderActive] = orange;

        colors[ImGuiCol_Tab] = bg0;
        colors[ImGuiCol_TabActive] = orange;
        colors[ImGuiCol_TabHovered] = bg2;

        colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_SliderGrab] = orange;
        colors[ImGuiCol_SliderGrabActive] =  ImVec4(orange.x, orange.y, orange.z, 0.8f);
        colors[ImGuiCol_CheckMark] = orange;

        colors[ImGuiCol_ResizeGrip] =  ImVec4(orange.x, orange.y, orange.z, 0.4f);
        colors[ImGuiCol_ResizeGripHovered] =  ImVec4(orange.x, orange.y, orange.z, 0.8f);
        colors[ImGuiCol_ResizeGripActive] = orange;
    }
    void ImGuiLayer::ApplyCyberpunkTheme()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        style.WindowPadding = ImVec2(10, 10);
        style.FramePadding = ImVec2(8, 5);
        style.ItemSpacing = ImVec2(10, 8);
        style.WindowRounding = 4.0f;
        style.FrameRounding = 3.0f;
        style.GrabRounding = 3.0f;
        style.TabRounding = 4.0f;

        ImVec4 black = ImVec4(0.05f, 0.05f, 0.08f, 1.00f);
        ImVec4 dark = ImVec4(0.10f, 0.10f, 0.15f, 1.00f);
        ImVec4 neonBlue = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
        ImVec4 neonPink = ImVec4(1.00f, 0.00f, 0.60f, 1.00f);
        ImVec4 neonPurple = ImVec4(0.60f, 0.00f, 1.00f, 1.00f);
        ImVec4 text = ImVec4(0.85f, 0.95f, 1.00f, 1.00f);

        colors[ImGuiCol_WindowBg] = dark;
        colors[ImGuiCol_ChildBg] = black;
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.12f, 0.95f);

        colors[ImGuiCol_Text] = text;
        colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.45f, 0.55f, 1.00f);

        colors[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.18f, 0.28f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(neonBlue.x, neonBlue.y, neonBlue.z, 0.25f);

        colors[ImGuiCol_Button] = ImVec4(0.12f, 0.12f, 0.20f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(neonBlue.x, neonBlue.y, neonBlue.z, 0.8f);
        colors[ImGuiCol_ButtonActive] = neonBlue;

        colors[ImGuiCol_Header] = ImVec4(0.15f, 0.15f, 0.25f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(neonPurple.x, neonPurple.y, neonPurple.z, 0.7f);
        colors[ImGuiCol_HeaderActive] = neonPurple;

        colors[ImGuiCol_Tab] = dark;
        colors[ImGuiCol_TabActive] = neonBlue;
        colors[ImGuiCol_TabHovered] = ImVec4(neonBlue.x, neonBlue.y, neonBlue.z, 0.7f);

        colors[ImGuiCol_Border] = ImVec4(neonBlue.x, neonBlue.y, neonBlue.z, 0.3f);
        colors[ImGuiCol_SliderGrab] = neonPink;
        colors[ImGuiCol_SliderGrabActive] = ImVec4(neonPink.x, neonPink.y, neonPink.z, 0.8f);
        colors[ImGuiCol_CheckMark] = neonBlue;

        colors[ImGuiCol_PlotLines] = neonBlue;
        colors[ImGuiCol_PlotHistogram] = neonPink;
    }
    void ImGuiLayer::ApplySoftLightTheme()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        // --- Sizing ---
        style.WindowPadding = ImVec2(10, 10);
        style.FramePadding = ImVec2(8, 6);
        style.ItemSpacing = ImVec2(10, 8);
        style.ItemInnerSpacing = ImVec2(6, 4);

        style.WindowRounding = 8.0f;
        style.FrameRounding = 6.0f;
        style.PopupRounding = 8.0f;
        style.ScrollbarRounding = 6.0f;
        style.GrabRounding = 4.0f;
        style.TabRounding = 6.0f;

        style.WindowBorderSize = 1.0f;
        style.FrameBorderSize = 0.0f;

        style.ScrollbarSize = 12.0f;
        style.GrabMinSize = 8.0f;

        // --- Color Palette ---
        ImVec4 white = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        ImVec4 bg0 = ImVec4(0.96f, 0.96f, 0.97f, 1.00f); // app background
        ImVec4 bg1 = ImVec4(0.92f, 0.92f, 0.94f, 1.00f); // window bg
        ImVec4 bg2 = ImVec4(0.88f, 0.88f, 0.90f, 1.00f); // frame bg
        ImVec4 bg3 = ImVec4(0.82f, 0.82f, 0.85f, 1.00f); // hovered

        ImVec4 accent = ImVec4(0.00f, 0.47f, 0.98f, 1.00f); // macOS blue
        ImVec4 accentH = ImVec4(0.20f, 0.55f, 1.00f, 1.00f);
        ImVec4 accentA = ImVec4(0.00f, 0.40f, 0.90f, 0.40f);

        ImVec4 text = ImVec4(0.15f, 0.15f, 0.18f, 1.00f);
        ImVec4 textD = ImVec4(0.50f, 0.50f, 0.55f, 1.00f);
        ImVec4 border = ImVec4(0.75f, 0.75f, 0.78f, 1.00f);

        // --- Backgrounds ---
        colors[ImGuiCol_WindowBg] = bg1;
        colors[ImGuiCol_ChildBg] = bg0;
        colors[ImGuiCol_PopupBg] = white;
        colors[ImGuiCol_Border] = border;
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

        // --- Text ---
        colors[ImGuiCol_Text] = text;
        colors[ImGuiCol_TextDisabled] = textD;

        // --- Frame ---
        colors[ImGuiCol_FrameBg] = bg2;
        colors[ImGuiCol_FrameBgHovered] = bg3;
        colors[ImGuiCol_FrameBgActive] = ImVec4(accent.x, accent.y, accent.z, 0.25f);

        // --- Button ---
        colors[ImGuiCol_Button] = bg2;
        colors[ImGuiCol_ButtonHovered] = bg3;
        colors[ImGuiCol_ButtonActive] = accentA;

        // --- Tab ---
        colors[ImGuiCol_Tab] = bg2;
        colors[ImGuiCol_TabHovered] = bg3;
        colors[ImGuiCol_TabActive] = white;
        colors[ImGuiCol_TabUnfocused] = bg1;
        colors[ImGuiCol_TabUnfocusedActive] = bg2;

        // --- Title ---
        colors[ImGuiCol_TitleBg] = bg1;
        colors[ImGuiCol_TitleBgActive] = bg2;
        colors[ImGuiCol_TitleBgCollapsed] = bg1;

        // --- Header ---
        colors[ImGuiCol_Header] = bg2;
        colors[ImGuiCol_HeaderHovered] = bg3;
        colors[ImGuiCol_HeaderActive] = accentA;

        // --- Resize / Scrollbar ---
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.60f, 0.60f, 0.65f, 0.30f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.40f, 0.40f, 0.45f, 0.60f);
        colors[ImGuiCol_ResizeGripActive] = accent;

        colors[ImGuiCol_ScrollbarBg] = bg1;
        colors[ImGuiCol_ScrollbarGrab] = bg3;
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.70f, 0.70f, 0.75f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.60f, 0.60f, 0.65f, 1.00f);

        // --- Interactive ---
        colors[ImGuiCol_CheckMark] = accent;
        colors[ImGuiCol_SliderGrab] = accent;
        colors[ImGuiCol_SliderGrabActive] = accentH;
        colors[ImGuiCol_PlotLines] = accent;
        colors[ImGuiCol_PlotHistogram] = accent;

        // --- Selection ---
        colors[ImGuiCol_TextSelectedBg] = accentA;
        colors[ImGuiCol_NavHighlight] = accent;
    }

    void ImGuiLayer::ApplyDarkTheme()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        // --- Sizing & Layout ---
        style.WindowPadding = ImVec2(10, 10);
        style.FramePadding = ImVec2(8, 5);
        style.CellPadding = ImVec2(6, 4);
        style.ItemSpacing = ImVec2(10, 6);
        style.ItemInnerSpacing = ImVec2(6, 4);

        style.WindowRounding = 6.0f;
        style.FrameRounding = 5.0f;
        style.PopupRounding = 6.0f;
        style.ScrollbarRounding = 6.0f;
        style.GrabRounding = 4.0f;
        style.TabRounding = 5.0f;

        style.WindowBorderSize = 1.0f;
        style.FrameBorderSize = 0.0f;
        style.PopupBorderSize = 1.0f;

        style.ScrollbarSize = 12.0f;
        style.GrabMinSize = 8.0f;

        // --- Colors ---
        ImVec4 bg0 = ImVec4(0.08f, 0.09f, 0.11f, 1.00f); // background
        ImVec4 bg1 = ImVec4(0.12f, 0.13f, 0.16f, 1.00f); // window bg
        ImVec4 bg2 = ImVec4(0.16f, 0.17f, 0.21f, 1.00f); // frame bg
        ImVec4 bg3 = ImVec4(0.22f, 0.24f, 0.29f, 1.00f); // hovered

        ImVec4 accent = ImVec4(0.26f, 0.59f, 0.98f, 1.00f); // primary blue
        ImVec4 accentH = ImVec4(0.35f, 0.65f, 1.00f, 1.00f); // hover
        ImVec4 accentA = ImVec4(0.26f, 0.59f, 0.98f, 0.35f); // active

        ImVec4 text = ImVec4(0.92f, 0.93f, 0.95f, 1.00f);
        ImVec4 textD = ImVec4(0.55f, 0.57f, 0.62f, 1.00f);

        // Backgrounds
        colors[ImGuiCol_WindowBg] = bg1;
        colors[ImGuiCol_ChildBg] = bg0;
        colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.11f, 0.14f, 0.98f);
        colors[ImGuiCol_Border] = ImVec4(0.18f, 0.20f, 0.25f, 0.80f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

        // Text
        colors[ImGuiCol_Text] = text;
        colors[ImGuiCol_TextDisabled] = textD;

        // Frame
        colors[ImGuiCol_FrameBg] = bg2;
        colors[ImGuiCol_FrameBgHovered] = bg3;
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.30f);

        // Buttons
        colors[ImGuiCol_Button] = bg2;
        colors[ImGuiCol_ButtonHovered] = bg3;
        colors[ImGuiCol_ButtonActive] = accentA;

        // Tabs
        colors[ImGuiCol_Tab] = bg2;
        colors[ImGuiCol_TabHovered] = bg3;
        colors[ImGuiCol_TabActive] = accent;
        colors[ImGuiCol_TabUnfocused] = bg1;
        colors[ImGuiCol_TabUnfocusedActive] = bg2;

        // Title
        colors[ImGuiCol_TitleBg] = bg0;
        colors[ImGuiCol_TitleBgActive] = bg1;
        colors[ImGuiCol_TitleBgCollapsed] = bg0;

        // Headers
        colors[ImGuiCol_Header] = bg2;
        colors[ImGuiCol_HeaderHovered] = bg3;
        colors[ImGuiCol_HeaderActive] = accentA;

        // Resize Grip
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.60f);
        colors[ImGuiCol_ResizeGripActive] = accent;

        // Scrollbar
        colors[ImGuiCol_ScrollbarBg] = bg1;
        colors[ImGuiCol_ScrollbarGrab] = bg3;
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.28f, 0.30f, 0.36f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.32f, 0.34f, 0.41f, 1.00f);

        // Checkbox / Slider / Plot
        colors[ImGuiCol_CheckMark] = accent;
        colors[ImGuiCol_SliderGrab] = accent;
        colors[ImGuiCol_SliderGrabActive] = accentH;
        colors[ImGuiCol_PlotLines] = accent;
        colors[ImGuiCol_PlotHistogram] = accent;

        // Misc
        colors[ImGuiCol_TextSelectedBg] = accentA;
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = accent;
            
        
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