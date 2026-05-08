#pragma once

namespace GT
{
    class FileSystem
    {
    public:
        virtual ~FileSystem() = default;

        virtual bool Exists(const std::filesystem::path& path) const = 0;
        virtual bool ReadFile(const std::filesystem::path& path, std::vector<uint8_t>& outData) = 0;
        virtual bool WriteFile(const std::filesystem::path& path, const std::vector<uint8_t>& data) = 0;
    };
}