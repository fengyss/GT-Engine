#pragma once
#include "gtpch.h"
//#include "Asset.h"
//
//namespace GT
//{
//    void AssetLibrary::Add(uint32_t ID, const Ref<Asset>& asset)
//    {
//        m_Assets[ID] = asset;
//    }
//
//    Ref<Asset> AssetLibrary::Load(uint32_t ID, const std::filesystem::path& filepath)
//    {
//        if (Exists(ID))
//        {
//            GT_CORE_INFO("As {1} with ID {0} already loaded!", ID, filepath.filename().string());
//            return m_Assets[ID];
//        }
//        else {
//            auto As = CreateRef<Asset>(filepath);
//            Add(ID, As);
//            return As;
//        }
//    }
//    Ref<Asset> AssetLibrary::Reload(uint32_t ID, const std::filesystem::path& filepath)
//    {
//        GT_CORE_WARN("As {1} with ID {0} reloaded!", ID, filepath.filename().string());
//        m_Assets[ID] = CreateRef<Asset>(filepath);
//        return m_Assets[ID];
//    }
//    Ref<Asset> AssetLibrary::Get(uint32_t ID)
//    {
//        if (Exists(ID)) return m_Assets[ID];
//        else
//        {
//            GT_CORE_ERROR("Asset ID:{0} not found in library!", ID);
//            return nullptr;
//        }
//    }
//}




