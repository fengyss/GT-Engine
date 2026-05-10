#pragma once

#include "AssetsManager.h"
#include "GT/Renderer/Texture.h"
#include "GT/Renderer/Shader.h"

namespace GT
{
    template<typename T>
    class AssetsHandle
    {
    public:
        AssetsHandle() = default;

        AssetsHandle(const char* name) : m_Name(name) {
            m_Cached = AssetsManager::Get<T>(name);
            m_Path = AssetsManager::GetPath<T>(name);

            AssetsManager::RegisterReloadCallback(m_Path, [this]() {
                    this->Invalidate();
                });
        }

        AssetsHandle(const std::filesystem::path& path) : m_Path(path) {
            m_Cached = AssetsManager::Load<T>(path);
            AssetsManager::RegisterReloadCallback(m_Path, [this]() {
                this->Invalidate();
                });
        }

        AssetsHandle(const std::string& name, const std::filesystem::path& path)
            : m_Name(name), m_Path(path) {
            m_Cached = AssetsManager::Load<T>(path);
            AssetsManager::RegisterReloadCallback(m_Path, [this]() {
                this->Invalidate();
                });
        }

        Ref<T> Get()
        {
            if (!m_Cached)
            {
                m_Cached = AssetsManager::Reload<T>(m_Path);
            }
            return m_Cached;
        }

        void Invalidate()
        {
            m_Cached = nullptr;
        }

        void SetPath(const std::filesystem::path& path)
        {
            m_Path = path;
        }

        bool IsValid() const
        {
            return !m_Path.empty();
        }

        const std::filesystem::path& GetPath() const
        {
            return m_Path;
        }

    private:
        std::string m_Name;
        std::filesystem::path m_Path;
        Ref<T> m_Cached = nullptr;
    };

    // Assets Handle Ref
    template<typename T>
    using RefHandle = std::shared_ptr<AssetsHandle<T>>;

    template<typename T, typename ... Args>
    constexpr RefHandle<T> CreateHandle(Args&& ... args)
    {
        return std::make_shared<AssetsHandle<T>>(std::forward<Args>(args)...);
    }

}