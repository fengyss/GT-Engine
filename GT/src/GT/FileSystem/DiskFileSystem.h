#pragma once

#include "FileSystem.h"

namespace GT
{
    class DiskFileSystem : public FileSystem
    {
    public:
        DiskFileSystem(const std::filesystem::path& rootPath)
            : m_RootPath(rootPath) {
        }

        bool Exists(const std::filesystem::path& path) const override
        {
            return std::filesystem::exists(m_RootPath /  path);
        }

        bool ReadFile(const std::filesystem::path& path, std::vector<uint8_t>& outData) override
        {
            std::ifstream file(m_RootPath / path, std::ios::binary);
            if (!file) return false;

            file.seekg(0, std::ios::end);
            size_t size = file.tellg();
            file.seekg(0, std::ios::beg);

            outData.resize(size);
            file.read(reinterpret_cast<char*>(outData.data()), size);
            return true;
        }

    private:
        std::filesystem::path m_RootPath;
    };
}
