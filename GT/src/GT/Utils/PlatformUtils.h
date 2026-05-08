#pragma once

#include <string>

namespace GT
{
	class FileDialogs
	{
	public:
		// if canceled return empty
		static std::filesystem::path OpenFile(const char* filter);
		static std::filesystem::path SaveFile(const char* filter);
	};

	class Time
	{
	public:
		static float GetTime();
	};
}