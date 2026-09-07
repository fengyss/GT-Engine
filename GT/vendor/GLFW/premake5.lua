project "GLFW"
	kind "StaticLib"
	language "C"
	staticruntime "On" 

targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

files { "./src/**.c","./src/**.h", "./include/**.h"} 



	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:linux"
		defines { "_GLFW_WAYLAND" }
		-- removefiles
		-- {
		-- 	"src/cocoa_*.c",
		-- 	"src/macos_*.c",
		-- 	"src/win32_*.c",
		-- 	"src/wgl_context.c",
		-- 	"src/x11_*.c",
		-- 	"src/xkb_unicode.c",
		-- }
		-- links { "wayland-client", "xkbcommon" }
	
	filter "system:windows"
		defines { "_GLFW_WIN32" }
		-- removefiles
		-- {
		-- 	"src/cocoa_*.c",
		-- 	"src/macos_*.c",
		-- 	"src/win32_*.c",
		-- 	"src/wgl_context.c",
		-- 	"src/x11_*.c",
		-- 	"src/xkb_unicode.c",
		-- 	"src/glx_context.c",
		-- 	"src/egl_context.c",
		-- 	"src/osmesa_context.c",
		-- }
		
filter "configurations:Debug"
defines { "DEBUG" }  
symbols "On" 

filter "configurations:Release"  
defines { "NDEBUG" }    
optimize "On" 