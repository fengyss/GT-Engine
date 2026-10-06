
project "Test"
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


	filter "system:linux"
		defines { "GT_PLATFORM_LINUX", "GT_BUILD_DLL" }
		linkoptions
		{
			"-LGT/vendor/assimp/lib/linux",
			"-LGT/vendor/mono/lib/linux",
			"-Wl,-l:libassimp.so",
			"-Wl,-l:libmono-2.0.so.1"
		}
		links
		{	
			"X11",
		}
	filter {}

	filter "system:windows"
		defines { "GT_PLATFORM_WINDOWS", "GT_BUILD_DLL" }
		links { "%{Library.assimp}", "%{Library.mono}" }
	filter {}

	
    filter {"system:windows", "configurations:Debug" }
		links
		{
			"LIBCMTD.lib"
		}
	filter {}


	libdirs {
        "%{LibraryDir.gtest}"
    }
	links {
        "%{Library.gtest}"
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
		