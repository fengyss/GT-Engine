project "gtest"
    kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

    
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")


    files {
        "include/**.cc",
        "include/**.h",
        "src/**.cc",
        "src/**.h",
    }

    includedirs
	{
        "include",
        ".",
	}

    -- defines { "GTEST_CREATE_SHARED_LIBRARY=0", "_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING" }
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