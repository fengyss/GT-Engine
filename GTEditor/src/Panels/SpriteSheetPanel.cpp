#include "SpriteSheetPanel.h"
#include "imgui/imgui.h"
#include <imgui/misc/cpp/imgui_stdlib.h>
#include "GT/Renderer/Texture.h"
#include "GT/Utils/PlatformUtils.h"

namespace GT {


    void SpriteSheetPanel::OnImGuiRender()
    {
        ImGui::Begin("Sprite Sheet Cropper");

        // 1. 拖拽加载纹理
        if (ImGui::Button("Load Texture"))
        {
            std::filesystem::path filepath = FileDialogs::OpenTextureFile();
            m_Texture = Texture2D::Create(filepath);
        }

        if (m_Texture)
        {
            ImGui::Image((void*)(intptr_t)m_Texture->GetRendererID(), ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));

            ImGui::Separator();
            ImGui::Text("Grid Slice Settings:");
            ImGui::DragInt("Rows", &m_GridRows, 1, 1, 32);
            ImGui::DragInt("Columns", &m_GridCols, 1, 1, 32);

            if (ImGui::Button("Slice by Grid"))
            {
                m_Regions = SpriteSheetCropper::SliceByGrid(m_Texture, m_GridRows, m_GridCols);
            }

            ImGui::SameLine();
            if (ImGui::Button("Slice by Content (Auto)"))
            {
                m_Regions = SpriteSheetCropper::SliceByContent(m_Texture, 1, 0.1f);
            }

            ImGui::Separator();
            ImGui::Text("Results: %zu Sprites found", m_Regions.size());

            // 2. 显示切片列表
            ImGui::BeginChild("SpriteList");
            for (size_t i = 0; i < m_Regions.size(); ++i)
            {
                ImGui::PushID(i);
                if (ImGui::Selectable(("Sprite_" + std::to_string(i)).c_str(), m_SelectedIndex == i))
                {
                    m_SelectedIndex = i;
                }
                ImGui::PopID();
            }
            ImGui::EndChild();

            // 3. 预览选中的 Sprite
            if (m_SelectedIndex >= 0 && m_SelectedIndex < (int)m_Regions.size())
            {
                ImGui::Separator();
                ImGui::Text("Preview:");
                const SpriteRegion& selected = m_Regions[m_SelectedIndex];

                // 计算预览 UV (需要翻转 Y 轴以符合 ImGui 的纹理坐标系)
                ImVec2 uv0(selected.UVOffset.x, 1.0f - selected.UVOffset.y - selected.UVSize.y);
                ImVec2 uv1(uv0.x + selected.UVSize.x, 1.0f - selected.UVOffset.y);
                ImGui::Image((void*)(intptr_t)m_Texture->GetRendererID(), ImVec2(64, 64), uv0, uv1);
            }

            // 4. 导出 JSON
            if (ImGui::Button("Export to JSON"))
            {
                SpriteSheetCropper::ExportToJSON(m_Regions, m_Texture->GetPath(), m_Texture->GetName());
            }
        }
        if (ImGui::Button("End"))
        {
            Close();
        }
        ImGui::End();
    }

}