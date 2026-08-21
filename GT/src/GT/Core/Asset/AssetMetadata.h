#pragma once
#include <filesystem>
#include "GT/Core/UUID.h"
namespace GT
{ 

    enum class AssetType : uint8_t
    {
        None = 0,
        Scene,
        Texture2D,
        Texture3D,
        Shader,
        ComputeShader,
        GeometryShader,
        Model
    };

    std::string_view AssetTypeToString(AssetType type);
    AssetType AssetTypeFromString(std::string_view assetType);

	struct AssetMetadata
	{
		AssetType Type = AssetType::None;
		std::string Name;
        UUID ID;
		std::filesystem::path FilePath;
        bool IsWatch = false;

		operator bool() const { return Type != AssetType::None; }
	};


};


