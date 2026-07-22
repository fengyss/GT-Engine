#include "gtpch.h"

#include "Asset.h"
#include "AssetImporter.h"
#include "TextureAssetImporter.h"
#include "ShaderAssetImporter.h"
#include "ModelAssetImporter.h"

namespace GT
{
	using AssetImportFunction = std::function<Ref<Asset>(const AssetMetadata&)>;

	static std::map<AssetType, AssetImportFunction> s_AssetImportFunctions = {
		{ AssetType::Texture2D, TextureAssetImporter::ImportTexture2D },
		{ AssetType::Shader, ShaderAssetImporter::ImportShader },
		{ AssetType::ComputeShader, ShaderAssetImporter::ImportShader },
		{ AssetType::GeometryShader, ShaderAssetImporter::ImportShader },
		{ AssetType::Model, ModelAssetImporter::ImportModel },
		//{ AssetType::Scene, SceneAssetImporter::ImportScene }
	};


	Ref<Asset> AssetImporter::ImportAsset(const AssetMetadata& metadata)
	{
		if (s_AssetImportFunctions.find(metadata.Type) == s_AssetImportFunctions.end())
		{
			GT_CORE_ERROR("No importer available for asset type: {}", AssetTypeToString(metadata.Type));
			return nullptr;
		}

		return s_AssetImportFunctions.at(metadata.Type)(metadata);
	}

}