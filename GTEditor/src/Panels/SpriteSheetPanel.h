#pragma once
#include "GT/Renderer/Texture.h"
#include "GT/Core/Animation/SpriteSheetCropper.h"

namespace GT {

    class SpriteSheetPanel
    {
    public:
        SpriteSheetPanel() = default;
         ~SpriteSheetPanel() = default;

        void OnImGuiRender();

        void Open() { m_IsOpen = true; }
        void Close() { m_IsOpen = false; }
        bool IsOpen() { return m_IsOpen; }

    private:
        Texture2D m_Texture;
        std::vector<SpriteRegion> m_Regions;

        bool m_IsOpen = false;

        // UI ¡Ÿ ±±‰¡ø
        int m_GridRows = 4;
        int m_GridCols = 4;
        int m_SelectedIndex = -1;
    };

}