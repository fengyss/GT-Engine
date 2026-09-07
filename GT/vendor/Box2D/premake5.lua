project "Box2D"
    kind "StaticLib"
    language "C"
    cdialect "C11"
    staticruntime "On"
    
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

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

    filter "system:linux"
        defines { "_POSIX_C_SOURCE=200809L" }

	filter "configurations:Debug"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		 optimize "On"
		 runtime "Release"
