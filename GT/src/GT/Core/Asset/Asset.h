#pragma once
#include <string>
#include <atomic>
#include <filesystem>
namespace GT {

    // 所有资源的基类
    class Asset {
    public:
        virtual ~Asset() = default;

        // 同步加载（子类实现）
        virtual bool Load(const std::filesystem::path& path) = 0;
        //virtual bool Get(const std::filesystem::path& path) = 0;

        // 卸载资源
        virtual void Unload() {}

        inline const std::filesystem::path& GetPath() const { return m_Path; }

    private:
        std::filesystem::path m_Path;
        std::atomic<bool> m_IsLoaded{ false };
    };

}