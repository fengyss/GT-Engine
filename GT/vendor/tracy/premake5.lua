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

defines 
{ 
    "TRACY_ENABLE", 
    --"TRACY_STATIC",
    --"TRACY_ON_DEMAND" 
}


    filter { "system:windows", "configurations:Debug" }
        symbols "On"
        editandcontinue "Off"   -- 生成 /Zi 而不是 /ZI
    filter {}

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
