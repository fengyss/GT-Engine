#pragma once
#include "Panel.h"
#include "GT/Renderer/Texture.h"

namespace GT
{

	class AssetsPanel : public Panel
	{
	public:
		AssetsPanel();
		void OnImGuiRender() override;
	private:
		Texture2D m_ModelIcon;
		Texture2D m_ShaderIcon;

		Texture2D m_SceneIcon;
		Texture2D m_TextureIcon;
	};

}