
project "GT"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	buildoptions "/utf-8"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "gtpch.h"
	pchsource "src/gtpch.cpp"

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

	includedirs
	{
		"src",
		"vendor",
		"vendor/spdlog/include",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGuizmo}",
		--"%{IncludeDir.mono}",
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
	}
	libdirs {
        "%{LibraryDir.freetype}",
    }
	links
	{
		"freetype.lib",
	}
	filter "files:vendor/ImGuizmo/**.cpp"
		flags { "NoPCH" }

	links
	{
		"efsw",
		"libcmt.lib",
		"GLFW",
		"Glad",
		"Box2D",
		"ENTT",
		"json",
		"yaml_cpp",
		"ImGui",
		"freetype",
		"opengl32.lib",
		--"%{Library.mono}"
	}

	filter "system:windows"
		systemversion "10.0"

		defines
		{
			"_CRT_SECURE_NO_WARNINGS",
			"GLFW_INCLUDE_NONE"
		}

		defines
		{
			"GT_PLATFORM_WINDOWS",
			"GT_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"YAML_CPP_STATIC_DEFINE"
		}
		links
		{
			"%{Library.WinSock}",
			"%{Library.WinMM}",
			"%{Library.WinVersion}",
			"%{Library.BCrypt}",
		}

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

    filter {"system:windows", "configurations:Release" }
		buildoptions "/MT"
		