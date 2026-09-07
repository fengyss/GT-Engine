
project "GTEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	filter "system:windows"
		buildoptions "/utf-8"
	filter {}


	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")


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
		"%{wks.location}/GT/vendor/ImGui",
		"%{wks.location}/GT/vendor/GLFW/include",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.efsw}",
	}

	libdirs 
	{
        "%{LibraryDir.assimp}",
		"%{wks.location}/GT/vendor/vld",
        "%{LibraryDir.mono}",
        "%{LibraryDir.freetype}",
        "%{LibraryDir.gtest}",
    }
	
	defines { "GLFW_INCLUDE_NONE", "YAML_CPP_STATIC_DEFINE" }


	filter "system:linux"
		defines { "GT_PLATFORM_LINUX", "GT_BUILD_DLL" }
		linkoptions
		{
			"-LGT/vendor/assimp/lib/linux",
			"-LGT/vendor/mono/lib/linux",
			"-Wl,-l:libassimp.so",
			"-Wl,-l:libmono-2.0.so.1"
		}
	filter {}

	filter "system:windows"
		defines { "GT_PLATFORM_WINDOWS", "GT_BUILD_DLL" }
		links { "%{Library.assimp}", "%{Library.mono}" }
	filter {}

	links
	{
		"GT",
		"efsw",
		"GLFW",
		"Glad",
		"Box2D",
		"ImGui",
		"yaml_cpp",
		
	}


	filter "system:windows"
		systemversion "10.0"
		
		linkoptions { "/ignore:4099" }
		linkoptions { "/ignore:4006" }
		linkoptions { "/ignore:4286" }

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