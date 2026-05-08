#pragma once

#include "GT/Renderer/Texture2D.h"
#include "GT/Assets/AssetsHandle.h"
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

		RefHandle<Texture2D> m_DirectoryIcon;
		RefHandle<Texture2D> m_FileIcon;
	};

}
