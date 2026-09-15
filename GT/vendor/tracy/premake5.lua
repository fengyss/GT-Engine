project "tracy"
kind "StaticLib"
language "C++"

targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")


files
{
    "public/tracy/Tracy.hpp",
    "public/TracyClient.cpp"
}

includedirs
{
    "public/tracy"
}

filter "system:windows"
systemversion "latest"
cppdialect "C++17"
staticruntime "on"

filter "system:linux"
pic "On"
systemversion "latest"
cppdialect "C++17"
staticruntime "off"

filter "configurations:Debug"
runtime "Debug"
symbols "on"

filter "configurations:Release"
runtime "Release"
optimize "on"
