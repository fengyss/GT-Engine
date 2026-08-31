#pragma once

#include "GT/Core/Base.h"
#include "GT/Renderer/Texture.h"
#include "AnimationTypes.h"

//#include <vector>
//#include <string>

namespace GT {

    

    class SpriteSheetCropper
    {
    public:
        SpriteSheetCropper() = default;
        ~SpriteSheetCropper() = default;

        // 1. 按固定行列网格切割 (最适合等宽等高)
        static std::vector<SpriteRegion> SliceByGrid(Texture2D& texture, uint32_t rows, uint32_t cols);

        // 2. 自动检测非透明像素边界进行切割 (适合不规则图集)
        // threshold: 透明度阈值 (0.0 - 1.0)
        static std::vector<SpriteRegion> SliceByContent(Texture2D& texture, uint32_t padding = 1, float alphaThreshold = 0.1f);

        // 3. 导出为 JSON 元数据 (Hazel 风格 Asset 序列化)
        static bool ExportToJSON(const std::vector<SpriteRegion>& regions, const std::filesystem::path& filepath, const std::string& textureName);
    };

}