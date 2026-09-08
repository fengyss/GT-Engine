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

		void OnTextureDisplay();
		void OnModelDisplay();
		void OnShaderDisplay();
		void OnSceneDisplay();
		void OnMaterialDisplay();
		void OnMeshDisplay();

		void ResetMeshIndex() { m_SelectedMeshIndex = -1; }

		// if no one is selected, return -1
		int GetSelectMeshIndex() { return m_SelectedMeshIndex; };
	private:
		// number of items to display per row
		int tex, sha, mod, scn, mat, mesh;
		int m_SelectedMeshIndex = -1;
		Texture2D m_ModelIcon;
		Texture2D m_ShaderIcon;

		Texture2D m_SceneIcon;
		Texture2D m_TextureIcon;
	};

}