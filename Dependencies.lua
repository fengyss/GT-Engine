
-- GT Dependencies

-- VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["stb_image"] = "%{wks.location}/GT/vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/GT/vendor/yaml-cpp/include"
-- IncludeDir["filewatch"] = "%{wks.location}/GT/vendor/filewatch"
IncludeDir["GLFW"] = "%{wks.location}/GT/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/GT/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/GT/vendor/ImGui"
IncludeDir["ImGuizmo"] = "%{wks.location}/GT/vendor/ImGuizmo"
IncludeDir["glm"] = "%{wks.location}/GT/vendor/glm"
IncludeDir["entt"] = "%{wks.location}/GT/vendor/entt"
IncludeDir["Box2D"] = "%{wks.location}/GT/vendor/Box2D/include"
IncludeDir["mono"] = "%{wks.location}/GT/vendor/mono/include"
IncludeDir["assimp"] = "%{wks.location}/GT/vendor/assimp/include"
IncludeDir["efsw"] = "%{wks.location}/GT/vendor/efsw/include"
IncludeDir["json"] = "%{wks.location}/GT/vendor/json/include"
IncludeDir["gtest"] = "%{wks.location}/GT/vendor/googletest/include"
-- IncludeDir["shaderc"] = "%{wks.location}/GT/vendor/shaderc/include"
-- IncludeDir["SPIRV_Cross"] = "%{wks.location}/GT/vendor/SPIRV-Cross"
-- IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
-- IncludeDir["msdfgen"] = "%{wks.location}/GT/vendor/msdf-atlas-gen/msdfgen"
-- IncludeDir["msdf_atlas_gen"] = "%{wks.location}/GT/vendor/msdf-atlas-gen/msdf-atlas-gen"

LibraryDir = {}

-- LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
--LibraryDir["mono"] = "%{wks.location}/GT/vendor/mono/lib/%{cfg.buildcfg}"
LibraryDir["assimp"] = "%{wks.location}/GT/vendor/assimp/lib"
LibraryDir["gtest"] = "%{wks.location}/GT/vendor/googletest/lib"

Library = {}
--Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"
Library["assimp"] = "%{LibraryDir.assimp}/assimp-vc143-mt.lib"
Library["gtest"] = "%{LibraryDir.gtest}/gtest.lib"

-- Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
-- Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

-- Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
-- Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
-- Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
-- Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

-- Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
-- Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
-- Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

--Windows
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"