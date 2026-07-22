#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>

namespace Utils {

    inline nlohmann::json LoadJSON(const std::filesystem::path& path)
    {
        nlohmann::json j;

        if (!std::filesystem::exists(path))
        {
            GT_CORE_ERROR("Json file {} not exited!",path.string());
            return j;
        }
        if (path.extension() != "json")
        {
            GT_CORE_ERROR("File {} is not a json file!", path.string());
            return j;
        }
        std::ifstream file(path);
        file >> j;
        file.close();
        return j;
    }

    inline void SaveJSON(const nlohmann::json& j, const std::filesystem::path& path)
    {
        std::ofstream file(path);
        file << j.dump(4);
        file.close();
    }

}