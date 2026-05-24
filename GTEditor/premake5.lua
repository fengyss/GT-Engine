
project "GTEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	buildoptions "/utf-8"


	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	ignoredefaultlibraries { "libcmtd" }

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		
		"%{wks.location}/GTEditor/src",
		"%{wks.location}/GT/vendor/spdlog/include",
		"%{wks.location}/GT/src",
		"%{wks.location}/GT/vendor",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.efsw}",
	}

	libdirs {
        "%{LibraryDir.assimp}",
		"%{wks.location}/GT/vendor/vld",
    }
	links {
        "assimp-vc143-mt.lib",
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
		"GT",
		"efsw",
		"GLFW",
		"Glad",
		"Box2D",
		"ImGui",
		"yaml_cpp",
		--"%{Library.mono}"
		
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "GT_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GT_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "GT_DIST"
		runtime "Release"
		optimize "on"