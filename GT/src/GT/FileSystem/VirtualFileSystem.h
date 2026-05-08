#pragma once

#include "FileSystem.h"

namespace GT
{
    class VirtualFileSystem
    {
    public:
        static VirtualFileSystem& Get()
        {
            static VirtualFileSystem instance;
            return instance;
        }

        // 挂载物理路径到虚拟路径
        void Mount(const std::string& virtualPath, Ref<FileSystem> fileSystem)
        {
            m_MountPoints[virtualPath] = fileSystem;
        }

        bool ReadFile(const std::string& virtualPath, std::vector<uint8_t>& outData)
        {
            for (auto& [prefix, fs] : m_MountPoints)
            {
                if (virtualPath.find(prefix) == 0)
                {
                    std::string relativePath = virtualPath.substr(prefix.length());
                    return fs->ReadFile(relativePath, outData);
                }
            }
            return false;
        }

    private:
        std::unordered_map<std::string, Ref<FileSystem>> m_MountPoints;
    };
}

