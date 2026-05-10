#include "gtpch.h"
#include "SpriteSheetCropper.h"
#include <algorithm>
#include <iostream>
#include <queue>
#include "stb_image.h"
#include "GT/Core/Log.h"

#define JSON_USE_IMPLICIT_CONVERSIONS 0
#include "nlohmann/json.hpp"

namespace GT {

    std::vector<SpriteRegion> SpriteSheetCropper::SliceByGrid(Ref<Texture2D> texture, uint32_t rows, uint32_t cols)
    {
        std::vector<SpriteRegion> regions;
        if (!texture) return regions;

        float texWidth = (float)texture->GetWidth();
        float texHeight = (float)texture->GetHeight();

        float cellWidth = texWidth / (float)cols;
        float cellHeight = texHeight / (float)rows;

        for (uint32_t r = 0; r < rows; ++r)
        {
            for (uint32_t c = 0; c < cols; ++c)
            {
                SpriteRegion region;
                // Hazel 的 UV 原点通常在左下角
                region.UVOffset = glm::vec2(c * cellWidth / texWidth, r * cellHeight / texHeight);
                region.UVSize = glm::vec2(cellWidth / texWidth, cellHeight / texHeight);

                region.PixelOffset = glm::vec2(c * cellWidth, r * cellHeight);
                region.PixelSize = glm::vec2(cellWidth, cellHeight);
                regions.push_back(region);
            }
        }
        return regions;
    }

    std::vector<SpriteRegion> SpriteSheetCropper::SliceByContent(Ref<Texture2D> texture, uint32_t padding, float alphaThreshold)
    {
        std::vector<SpriteRegion> regions;
        if (!texture) return regions;

        // 1. 获取纹理像素数据 (注意：某些 API 如 OpenGL 需要手动翻转 Y 轴)
        //uint32_t width = texture->GetWidth();
        //uint32_t height = texture->GetHeight();
        //Ref<Image> image = Image::Create(texture->GetPath()); // 假设你有 Image 类能直接读文件
        //const uint8_t* pixels = image->GetPixelData(); // RGBA 格式

        Image image = texture->GetData();
        const uint8_t* pixels = image.data;
        GT_CORE_ASSERT(image.channels == 4, "Only support image of 4 channels");
        uint32_t width = image.width;
        uint32_t height = image.height;

        // 2. 创建一个二维标记数组，记录每个像素是否属于某个已发现的精灵
        std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));

        for (uint32_t y = 0; y < height; ++y)
        {
            for (uint32_t x = 0; x < width; ++x)
            {
                // 获取当前像素透明度
                int index = (y * width + x) * 4;
                float alpha = (float)pixels[index + 3] / 255.0f;

                // 如果是不透明的，且未被访问过，说明找到了一个新精灵的边界
                if (alpha > alphaThreshold && !visited[y][x])
                {
                    int minX = x, minY = y, maxX = x, maxY = y;

                    // 使用 BFS 或 DFS 找到这个连通域的边界
                    std::queue<std::pair<int, int>> q;
                    q.push({ x, y });
                    visited[y][x] = true;

                    while (!q.empty())
                    {
                        auto [cx, cy] = q.front(); q.pop();
                        minX = std::min(minX, cx); maxX = std::max(maxX, cx);
                        minY = std::min(minY, cy); maxY = std::max(maxY, cy);

                        // 检查 4 个邻居
                        int dx[] = { -1, 1, 0, 0 };
                        int dy[] = { 0, 0, -1, 1 };
                        for (int i = 0; i < 4; ++i)
                        {
                            int nx = cx + dx[i], ny = cy + dy[i];
                            if (nx >= 0 && nx < (int)width && ny >= 0 && ny < (int)height)
                            {
                                int nIdx = (ny * width + nx) * 4;
                                float nAlpha = (float)pixels[nIdx + 3] / 255.0f;
                                if (nAlpha > alphaThreshold && !visited[ny][nx])
                                {
                                    visited[ny][nx] = true;
                                    q.push({ nx, ny });
                                }
                            }
                        }
                    }

                    // 应用 Padding 并 clamp 到纹理边界
                    minX = std::max(0, minX - (int)padding);
                    minY = std::max(0, minY - (int)padding);
                    maxX = std::min((int)width - 1, maxX + (int)padding);
                    maxY = std::min((int)height - 1, maxY + (int)padding);

                    // 转换为 Hazel 的 UV 坐标 (原点在左下角)
                    SpriteRegion region;
                    region.PixelOffset = glm::vec2(minX, minY);
                    region.PixelSize = glm::vec2(maxX - minX + 1, maxY - minY + 1);

                    region.UVOffset.x = (float)minX / (float)width;
                    region.UVOffset.y = 1.0f - ((float)maxY + 1.0f) / (float)height; // 翻转 Y
                    region.UVSize.x = (float)region.PixelSize.x / (float)width;
                    region.UVSize.y = (float)region.PixelSize.y / (float)height;

                    regions.push_back(region);
                }
            }
        }

        return regions;
    }

    bool SpriteSheetCropper::ExportToJSON(const std::vector<SpriteRegion>& regions, const std::filesystem::path& filepath, const std::string& textureName)
    {
        using json = nlohmann::json;

        json root;
        root["Texture"] = textureName;
        root["Sprites"] = json::array();

        for (size_t i = 0; i < regions.size(); ++i)
        {
            const auto& region = regions[i];

            json sprite;
            sprite["Name"] = "Sprite_" + std::to_string(i);
            sprite["X"] = region.PixelOffset.x;
            sprite["Y"] = region.PixelOffset.y;
            sprite["W"] = region.PixelSize.x;
            sprite["H"] = region.PixelSize.y;
            sprite["U"] = region.UVOffset.x;
            sprite["V"] = region.UVOffset.y;
            sprite["USize"] = region.UVSize.x;
            sprite["VSize"] = region.UVSize.y;

            root["Sprites"].push_back(sprite);
        }

        // 写入文件（4空格缩进，更易读）
        std::filesystem::path path = filepath;
        path.replace_extension(".json");

        std::ofstream file(path);
        if (!file.is_open()) return false;

        file << root.dump(4);
        file.close();

        return true;
    }

}