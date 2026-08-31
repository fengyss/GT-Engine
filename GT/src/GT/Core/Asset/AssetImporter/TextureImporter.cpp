#include "gtpch.h"

#include "TextureImporter.h"
#include "../TextureAsset.h"
#include "../AssetManager.h"
namespace GT
{
	Ref<AssetMetadata> TextureImporter::GeneratorMetadataFromPath(const std::filesystem::path& path)
	{
		Ref<AssetMetadata> meta = CreateRef<AssetMetadata>();
		meta->FilePath = path;
		meta->Name = path.stem().string();
		meta->ID = UUID();
		meta->Type = AssetType::Texture2D;
		return meta;
	}


	Ref<Asset> TextureImporter::ImportTexture2D(const Ref<AssetMetadata> metadata)
	{

#if GT_DEBUG
		if (!AssetManager::ExistedUUID(metadata->ID)) 
			AssetManager::RegisterMetadata(*metadata);
#endif

		Ref<Texture2DAsset> texture = Texture2DAsset::Create(metadata->FilePath);

		texture->ID = metadata->ID;
		texture->Name = metadata->Name;

		return texture;
	}


	Ref<Asset> TextureImporter::ImportTexture2D(const std::filesystem::path& path)
	{
		Ref<AssetMetadata> metadata = GeneratorMetadataFromPath(path);

		AssetManager::SaveMetadata(metadata);
		AssetManager::RegisterMetadata(*metadata);

		return ImportTexture2D(metadata);

	}

	Ref<Asset> TextureImporter::ReloadTexture2D(const std::filesystem::path& path)
	{
		return Texture2DAsset::Create(path);;
	}


	
	
}