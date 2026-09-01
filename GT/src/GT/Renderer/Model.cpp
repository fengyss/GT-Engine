#include "gtpch.h"


#include "GT/Core/Asset/AssetMetadata.h"
#include "GT/Core/Asset/ModelAsset.h"

#include "GT/Core/Asset/AssetManager.h"

#include "Model.h"


namespace GT
{
	Model::Model(const Model& asset)
	{
		if (asset.IsValid)
		{
			handle = AssetManager::GetAssetHandle(asset.handle.ID);
			IsValid = AssetManager::Existed(handle);

		}
		else
		{
			handle = Handle();
			IsValid = false;
		}

	}
	Model::Model(const std::filesystem::path& path)
	{
		if (path.has_extension())
		{
			handle.ID = AssetManager::RegisterModelAsset(path);
			handle = AssetManager::GetAssetHandle(handle.ID);
		}
		else handle = AssetManager::GetAssetHandle(path.string());

		IsValid = AssetManager::Existed(handle);

	}

	Model::~Model()
	{
		if (IsValid)
		{
			AssetManager::ReleaseHandle(handle);
			IsValid = false;
		}
	}



	Ref<ModelAsset> Model::Get() const
	{
		auto Model = AssetManager::GetAsset(handle);

		if (!Model)
		{
			GT_CORE_ERROR("Model is unknown : {0}", handle);
			Model = AssetManager::GetDefaultAsset(AssetType::Model);
		}
		return std::dynamic_pointer_cast<ModelAsset>(Model);
	}

	void Model::Reset(const Handle& hdl)
	{
		handle = hdl;
		IsValid = AssetManager::Existed(handle);
	}

	Model& Model::operator=(const Model& rhs)
	{

		if (this != &rhs)
		{
			AssetManager::ReleaseHandle(handle);
			handle = AssetManager::GetAssetHandle(rhs.handle.ID);
			IsValid = AssetManager::Existed(handle);
		}
		return *this;
	}

}