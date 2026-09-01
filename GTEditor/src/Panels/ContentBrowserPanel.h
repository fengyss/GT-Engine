#pragma once

#include "GT/Renderer/Texture.h"
#include <filesystem>

namespace GT {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();
	private:
		std::filesystem::path m_BaseDirectory;
		std::filesystem::path m_CurrentDirectory;

		Texture2D m_DirectoryIcon;
		Texture2D m_FileIcon;
	};

}
