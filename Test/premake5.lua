
project "Test"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	buildoptions "/utf-8"
	buildoptions "/NODEFAULTLIB:msvcrtd.lib"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")


	files
	{
		
		"**.h",
		"**.cc",
	}
	includedirs
	{
        "%{wks.location}/GTEditor/src",
		"%{wks.location}/GT/vendor/spdlog/include",
		"%{wks.location}/GT/src",
		"%{wks.location}/GT",
		"%{wks.location}/GT/vendor",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.efsw}",
		"%{IncludeDir.gtest}",
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
		"gtest",
	}

	libdirs {
        "%{LibraryDir.gtest}"
    }
	links {
        "%{Library.gtest}"
    }

	filter "system:windows"
		systemversion "10.0"

		defines
		{
		}

		defines
		{
			"GT_PLATFORM_WINDOWS"
		}
		links
		{
		}

	

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
		