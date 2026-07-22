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

        Handle handle;

        bool IsValid() const { return handle.generation != 0; }

        Ref<T> Get();


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

    template<typename T>
    Ref<T> AssetHandle<T>::Get()
    {
        return std::dynamic_pointer_cast<T>(AssetManager::GetAsset(this->handle));
    }

}