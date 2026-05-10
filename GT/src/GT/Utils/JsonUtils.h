#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>

namespace Utils {

    inline nlohmann::json LoadJSON(const std::filesystem::path& path)
    {
        std::ifstream file(path);
        nlohmann::json j;
        file >> j;
        return j;
    }

    inline void SaveJSON(const nlohmann::json& j, const std::filesystem::path& path)
    {
        std::ofstream file(path);
        file << j.dump(4);
    }

}