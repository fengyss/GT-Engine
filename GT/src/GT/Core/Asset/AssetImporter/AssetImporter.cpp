#include "gtpch.h"


namespace GT
{
	//using AssetImportFunction = std::function<Ref<Asset>(const Ref<AssetMetadata>)>;

	//static std::map<AssetType, AssetImportFunction> s_AssetImportFunctions = {
	//	//{ AssetType::Texture2D, TextureImporter::ImportTexture2D },
	//	//{ AssetType::Shader, ShaderImporter::ImportShader },
	//	//{ AssetType::ComputeShader, ShaderImporter::ImportShader },
	//	//{ AssetType::GeometryShader, ShaderImporter::ImportShader },
	//	//{ AssetType::Model, ModelImporter::ImportModel },
	//	//{ AssetType::Scene, SceneImporter::ImportScene }
	//};


	//Ref<Asset> AssetImporter::ImportAsset(const AssetMetadata& metadata)
	//{
	//	if (s_AssetImportFunctions.find(metadata.Type) == s_AssetImportFunctions.end())
	//	{
	//		GT_CORE_ERROR("No importer available for asset type: {}", AssetTypeToString(metadata.Type));
	//		return nullptr;
	//	}
	//	//return s_AssetImportFunctions.at(metadata.Type)(metadata);
	//}

}