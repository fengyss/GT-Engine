#pragma once

#include "GT/ImGui/ImGuiLayer.h"
#include "GT/Renderer/Texture.h"
#include "GT/Core/Animation/SpriteSheetCropper.h"

namespace GT {

    class SpriteSheetPanel : public Layer
    {
    public:
        SpriteSheetPanel();
        virtual ~SpriteSheetPanel() = default;

        virtual void OnAttach() override;
        virtual void OnImGuiRender() override;

    private:
        Ref<Texture2D> m_Texture;
        std::vector<SpriteRegion> m_Regions;

        // UI ¡Ÿ ±±‰¡ø
        int m_GridRows = 4;
        int m_GridCols = 4;
        int m_SelectedIndex = -1;
    };

}