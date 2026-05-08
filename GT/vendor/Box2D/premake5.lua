project "Box2D"
    kind "StaticLib"
    language "C"
    cdialect "C11"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/box2d/**.h",
        "src/**.c",
        "src/**.h",
    }
    includedirs
	{
		"include"
	}

    filter "system:windows"
        systemversion "10.0"

	filter "configurations:Debug"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		 optimize "On"
		 runtime "Release"
