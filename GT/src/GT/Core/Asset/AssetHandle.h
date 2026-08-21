#pragma once
#include <array>
#include "GT/Core/Base.h"

#include "GT/Renderer/Texture.h"
#include "GT/Renderer/Shader.h"

#include "AssetManager.h"

namespace GT
{

    template<typename T>
    struct AssetHandle
    {
        AssetHandle(const char* name);
        AssetHandle(const std::filesystem::path& path, bool IsWatch = false);
        AssetHandle(const std::filesystem::path& path, const char* name, bool IsWatch = false);

        ~AssetHandle() { AssetManager::ReleaseAsset(handle); };

        Handle handle;

        bool IsValid() const { return handle.generation != 0; }

        Ref<T> Get();

        operator bool() const { return IsValid(); }
        AssetHandle<T> operator= (const AssetHandle<T>& other) { handle = other.handle; AssetManager::CopyHandle(handle); return *this; }
    };

    // Assets Handle Ref
    template<typename T>
    using RefHandle = std::shared_ptr<AssetHandle<T>>;

    template<typename T, typename ... Args>
    constexpr RefHandle<T> CreateHandle(Args&& ... args)
    {
        return std::make_shared<AssetHandle<T>>(std::forward<Args>(args)...);
    };


    template<typename T>
    AssetHandle<T>::AssetHandle(const char* name)
    {
        handle = AssetManager::GetHandle(name);
    }
    template<typename T>
    AssetHandle<T>::AssetHandle(const std::filesystem::path& path, bool IsWatch)
    {
        handle = AssetManager::LoadAsset(path, "", IsWatch);
    }
    template<typename T>
    AssetHandle<T>::AssetHandle(const std::filesystem::path& path, const char* name, bool IsWatch)
    {
        handle = AssetManager::LoadAsset(path,name, IsWatch);
    }

    // if failed will return default type asset based on T
    template<typename T>
    Ref<T> AssetHandle<T>::Get()
    {
        auto asset = AssetManager::GetAsset(this->handle);
        if(asset) return std::dynamic_pointer_cast<T>(asset);

        GT_CORE_ERROR("Try to get a not exited asset, returned a default asset!");

        AssetType type = AssetType::Shader;
        if constexpr (std::is_same<T,GT::Shader>::value) type = AssetType::Shader;
        if constexpr (std::is_same<T, GT::Texture2D>::value) type = AssetType::Texture2D;
        //if (std::is_same<T, GT::Texture3D>::value) type = AssetType::Texture3D;

        asset = AssetManager::GetDefaultAsset(type);

        return std::dynamic_pointer_cast<T>(asset);
    }

}