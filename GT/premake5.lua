
project "GT"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	filter "system:windows"
		buildoptions "/utf-8"
	filter {}

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "gtpch.h"
	pchsource "src/gtpch.cpp"
	enablepch "On"

	filter "system:linux"
		enablepch "Off"
	filter {}

	files
	{
		
		"src/**.h",
		"src/**.cpp",

		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",

		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",

		"vendor/ImGuizmo/**.h",
		"vendor/ImGuizmo/**.cpp",

		"vendor/assimp/include/assimp/**.hpp",
		"vendor/assimp/include/assimp/**.h",

		--"vendor/entt/entt.hpp",

		"vendor/mono/include/**.h",
		"vendor/ft2build.h",
	}

	
	filter "files:vendor/ImGuizmo/**.cpp"
		enablepch "Off"
	filter {}


	includedirs
	{
		"src",
		"vendor",
		"vendor/spdlog/include",
		"vendor/freetype",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.mono}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.efsw}",
		"%{IncludeDir.json}",
		"%{IncludeDir.tracy}",
	}
	libdirs {
        "%{LibraryDir.freetype}",
        "%{LibraryDir.mono}",
		"%{LibraryDir.assimp}",

    }



	links
	{
		"GTScriptCore",
		"efsw",
		"libcmt.lib",
		"GLFW",
		"Glad",
		"Box2D",
		"yaml_cpp",
		"tracy",
		"ImGui",
		"opengl32.lib",
		"%{Library.mono}",
		"%{Library.assimp}",
	}
	defines
	{
		"GLFW_INCLUDE_NONE",
		"YAML_CPP_STATIC_DEFINE",
		"TRACY_ENABLE",
		"TRACT_ON_DEMAND", 
		--"TRACY_STATIC"
	}
	filter "system:windows"
		systemversion "10.0"
		defines
		{
			"_CRT_SECURE_NO_WARNINGS",
			"GT_PLATFORM_WINDOWS",
		}

	filter "system:linux"
		defines 
		{ 
			"GT_PLATFORM_LINUX",
		}

	
	filter "system:windows"
		systemversion "10.0"
		
		disablewarnings { "4828" }
		disablewarnings { "4305" }
		disablewarnings { "4267" }

--	doesn't need to copy dlls anymore since we are using static lib
--	postbuildcommands
--	{
--		("{COPYFILE} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
--	}

	filter "configurations:Debug"
		defines "GT_DEBUG"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		 defines "GT_RELEASE"
		 optimize "On"
		 runtime "Release"

	filter "configurations:Dist"
		 defines "GT_DIST"
		 optimize "On"
		 runtime "Release"

		
