project "GLFW"
	kind "StaticLib"
	language "C"
	staticruntime "On" 

targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

files { "src/**.c","src/**.h", "include/**.h"} 



	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:linux"
		defines { "_GLFW_WAYLAND" } --,"_GLFW_X11" }
	
	filter "system:windows"
		defines { "_GLFW_WIN32" }
		
filter "configurations:Debug"
defines { "DEBUG" }  
symbols "On" 

filter "configurations:Release"  
defines { "NDEBUG" }    
optimize "On" 