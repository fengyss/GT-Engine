#pragma once

#include <nlohmann/json.hpp>

#include "GT/Core/Asset/AssetMetadata.h"

namespace Utils {

    inline nlohmann::json MetadataToJson(const GT::AssetMetadata& meta)
    {
        nlohmann::json j;
  //      j["ID"] = meta.ID;
  //      j["fileName"] = meta.fileName;
  //      j["relativePath"] = meta.relativePath;
  //      j["friendlyName"] = meta.friendlyName;
  //      j["tags"] = meta.tags;
  //      j["directDependencies"] = meta.directDependencies;
  //      j["version"] = meta.version;
  //      j["sourceHash"] = meta.sourceHash;
  //      j["lastModifiedTime"] = meta.lastModifiedTime;
  //      if (meta.textureSettings)
  //          j["textureSettings"] = *meta.textureSettings;
  //      if (meta.modelSettings)
  //          j["modelSettings"] = *meta.modelSettings;
  //      if (meta.soundSettings)
  //          j["soundSettings"] = *meta.soundSettings;
  //      if (meta.shaderSettings)
  //          j["shaderSettings"] = *meta.shaderSettings;
  //      if (meta.materialSettings)
  //          j["materialSettings"] = *meta.materialSettings;
  //      j["editorNote"] = meta.editorNote;
  //      j["Type"] = static_cast<int>(meta.Type);
  //      j["Name"] = meta.Name;
  //      j["FilePath"] = meta.FilePath.string();
  //      j["IsWatch"] = meta.IsWatch;
		return j;
	}
    inline GT::AssetMetadata JsonToMetadata(const nlohmann::json& file)
    {
        return GT::AssetMetadata();
    }
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