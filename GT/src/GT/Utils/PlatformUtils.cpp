#include "gtpch.h"
#include "PlatformUtils.h"

namespace GT
{
	std::filesystem::path FileDialogs::OpenTextureFile()
	{
		return OpenFile("Image Files (*.png;*.jpg;*.jpeg;*.bmp)\0*.png;*.jpg;*.jpeg;*.bmp\0");
	}

	std::filesystem::path FileDialogs::OpenShaderFile()
	{
		return OpenFile("Shader Files (*.glsl;*.hlsl)\0*.glsl;*.hlsl\0");
	}

	std::filesystem::path FileDialogs::OpenSceneFile()
	{
		return OpenFile("Scene Files (*.scene)\0*.scene\0");
	}
}
