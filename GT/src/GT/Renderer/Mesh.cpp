#include "gtpch.h"


#include "GT/Core/Asset/AssetMetadata.h"
#include "GT/Core/Asset/AssetManager.h"

#include "Mesh.h"


namespace GT
{
	Mesh::Mesh(const Mesh& asset)
	{
		if (asset.IsValid)
		{
			handle = AssetManager::GetAssetHandle(asset.handle.ID);
			IsValid = AssetManager::Existed(handle);

			GT_CORE_TRACE("Mesh(const Mesh&): {0}.", handle);
		}
		else
		{
			handle = Handle();
			IsValid = false;
		}

	}
	Mesh::Mesh(const std::vector<Vertex>& _vertices, const std::vector<unsigned int>& _indices, const std::vector<Texture2D>& _textures)
	{
		handle.ID = AssetManager::RegisterMeshAsset(_vertices, _indices, _textures);
		handle = AssetManager::GetAssetHandle(handle.ID);
		IsValid = AssetManager::Existed(handle);


		GT_CORE_TRACE("Mesh(): {0}.", handle);
	}

	Mesh::~Mesh()
	{
		if (IsValid)
		{
			GT_CORE_TRACE("~Mesh: {0}.", handle);
			AssetManager::ReleaseHandle(handle);
			IsValid = false;
		}
	}



	Ref<MeshAsset> Mesh::Get() const
	{
		auto Mesh = AssetManager::GetAsset(handle);

		if (!Mesh)
		{
			GT_CORE_ERROR("Mesh is unknown : {0}", handle);
			Mesh = AssetManager::GetDefaultAsset(AssetType::Mesh);
		}
		return std::dynamic_pointer_cast<MeshAsset>(Mesh);
	}

	void Mesh::Reset(const Handle& hdl)
	{
		handle = hdl;
		IsValid = AssetManager::Existed(handle);
	}

	Mesh& Mesh::operator=(const Mesh& rhs)
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