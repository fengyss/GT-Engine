#pragma once

#include <string>
#include <filesystem>

namespace GT
{
	class FileDialogs
	{
	public:
		// if canceled return empty
		static std::filesystem::path OpenFile(const char* filter);
		static std::filesystem::path SaveFile(const char* filter);


		static std::filesystem::path OpenTextureFile();
		static std::filesystem::path OpenShaderFile();
		static std::filesystem::path OpenSceneFile();
	};

	class Time
	{
	public:
		static float GetTime();
	};
}