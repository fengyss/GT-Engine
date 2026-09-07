project "json"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"
    
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "json/**.hpp",
    }

    filter "system:windows"
        systemversion "10.0"

	filter "configurations:Debug"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		 optimize "On"
		 runtime "Release"
