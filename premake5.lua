include "./vendor/premake/premake_customization/solution_items.lua"

workspace "GT"
	architecture "x86_64"
	startproject "GTEditor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Dependencies.lua"

group "Dependencies"
	include "vendor/premake"
	include "GT/vendor/Box2D"
	include "GT/vendor/GLFW"
	include "GT/vendor/Glad"
	include "GT/vendor/entt"
	include "GT/vendor/ImGui"
	include "GT/vendor/yaml-cpp"
	include "GT/vendor/efsw"
	include "GT/vendor/json"
	include "GT/vendor/googletest"
	include "GT/vendor/freetype"
group ""

group "Core"
	include "GT"
	-- include "GT-ScriptCore"
group ""

group "Tools"
	include "GTEditor"
	include "Test"
group ""

-- group "Misc"
-- 	include "Sandbox"
-- group ""