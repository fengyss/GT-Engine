#pragma once
#include "gtpch.h"
#include "GT/Utils/PlatformUtils.h"

#ifdef GT_PLATFORM_WINDOWS
#include <commdlg.h>
#else
#include <chrono>
#endif
#ifdef GT_PLATFORM_LINUX
#include <cstdio>
#include <sys/wait.h>
#endif
#include <GLFW/glfw3.h>
#ifdef GT_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

#include "GT/Core/Application.h"

namespace GT
{
	#ifdef GT_PLATFORM_LINUX
	namespace
	{
		std::string ShellQuote(const std::string& value)
		{
			std::string quoted = "'";
			for (char character : value)
			{
				if (character == '\'')
					quoted += "'\\''";
				else
					quoted += character;
			}
			quoted += "'";
			return quoted;
		}

		std::string FormatFileFilter(const char* filter, bool zenity)
		{
			if (!filter || !*filter)
				return {};

			std::string name(filter);
			const char* rawPatterns = filter + name.size() + 1;
			std::string patterns(rawPatterns);
			std::replace(patterns.begin(), patterns.end(), ';', ' ');

			if (zenity)
				return name + " | " + patterns;
			return name + " (" + patterns + ")";
		}

		std::filesystem::path RunFileDialogCommand(const std::string& command, bool& commandMissing)
		{
			commandMissing = false;
			FILE* process = popen(command.c_str(), "r");
			if (!process)
				return {};

			std::string result;
			char buffer[512];
			while (fgets(buffer, sizeof(buffer), process))
				result += buffer;

			const int status = pclose(process);
			if (status == -1)
				return {};
			if (WIFEXITED(status) && WEXITSTATUS(status) == 127)
			{
				commandMissing = true;
				return {};
			}
			if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
				return {};

			while (!result.empty() && (result.back() == '\n' || result.back() == '\r'))
				result.pop_back();
			return result.empty() ? std::filesystem::path{} : std::filesystem::path(result);
		}

		std::filesystem::path ShowLinuxFileDialog(bool save, const char* filter)
		{
			const std::string title = save ? "Save File" : "Open File";
			const std::string zenityFilter = FormatFileFilter(filter, true);
			std::string zenity = "zenity --file-selection --title=" + ShellQuote(title);
			if (save)
				zenity += " --save --confirm-overwrite";
			if (!zenityFilter.empty())
				zenity += " --file-filter=" + ShellQuote(zenityFilter);
			zenity += " 2>/dev/null";

			bool commandMissing = false;
			auto result = RunFileDialogCommand(zenity, commandMissing);
			if (!commandMissing)
				return result;

			const std::string kdialogFilter = FormatFileFilter(filter, false);
			std::string kdialog = save
				? "kdialog --getsavefilename . "
				: "kdialog --getopenfilename . ";
			kdialog += ShellQuote(kdialogFilter.empty() ? "All Files (*)" : kdialogFilter);
			kdialog += " --title " + ShellQuote(title) + " 2>/dev/null";
			return RunFileDialogCommand(kdialog, commandMissing);
		}
	}
	#endif

	#ifndef GT_PLATFORM_WINDOWS
	#ifdef GT_PLATFORM_LINUX
	std::filesystem::path FileDialogs::OpenFile(const char* filter) { return ShowLinuxFileDialog(false, filter); }
	std::filesystem::path FileDialogs::SaveFile(const char* filter) { return ShowLinuxFileDialog(true, filter); }
	#else
	std::filesystem::path FileDialogs::OpenFile(const char*) { return {}; }
	std::filesystem::path FileDialogs::SaveFile(const char*) { return {}; }
	#endif
	float Time::GetTime()
	{
		static const auto start = std::chrono::steady_clock::now();
		return std::chrono::duration<float>(std::chrono::steady_clock::now() - start).count();
	}
	#else
	std::filesystem::path FileDialogs::OpenFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::string();
	}
	std::filesystem::path FileDialogs::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// Sets the default extension by extracting it from the filter
		ofn.lpstrDefExt = strchr(filter, '\0') + 1;

		if (GetSaveFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::string();
	}

	
	
	float Time::GetTime()
	{
		return glfwGetTime();
	}
	#endif
}