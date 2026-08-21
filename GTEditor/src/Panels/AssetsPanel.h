#pragma once
#include "Panel.h"
#include "GT/Core/Asset/AssetHandle.h"

namespace GT
{
	class AssetsPanel : public Panel
	{
	public:
		AssetsPanel();
		void OnImGuiRender() override;
	private:
		RefHandle<Texture2D> m_ModelIcon;
		RefHandle<Texture2D> m_ShaderIcon;

		RefHandle<Texture2D> m_SceneIcon;
		RefHandle<Texture2D> m_TextureIcon;
	};

}