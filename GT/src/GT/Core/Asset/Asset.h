#pragma once
#include <string>
#include <atomic>
#include <filesystem>
#include "GT/Core/Log.h"
namespace GT {

    // 所有资源的基类
    class Asset {
    public:
        virtual ~Asset() = default;

        // 同步加载（子类实现）
        //virtual bool Load(const std::filesystem::path& path) = 0;
        //virtual bool Get(const std::filesystem::path& path) = 0;

        // 卸载资源
        //virtual void Unload() {}


        //inline const std::filesystem::path& GetPath() const { return m_Path; }

    protected:
        //std::filesystem::path m_Path;
        //Ref<Asset> m_Cache = nullptr;
    };

    class AssetLibrary
    {
    public:

        Ref<Asset> Load(uint32_t ID, const std::filesystem::path& filepath);
        Ref<Asset> Reload(uint32_t ID, const std::filesystem::path& filepath);
        void Clear() { m_Assets.clear(); }
        Ref<Asset> Get(uint32_t ID);

        bool Exists(uint32_t ID) const
        {
            return m_Assets.find(ID) != m_Assets.end();
        }
    private:
        void Add(uint32_t ID, const Ref<Asset>& Asset);
        std::unordered_map<uint32_t, Ref<Asset>> m_Assets;
    };
}