#include "gtpch.h"
#include "AssetManager.h"
#include "AssetImporter/AssetImporter.h"
#include "GT/Utils/JsonUtils.h"
#include "GT/Project/Project.h"
#include "GT/Math/Math.h"
#include "Handle.h"

#include "GT/Renderer/Texture.h"
#include "GT/Renderer/Shader.h"
#include "GT/Scene/Scene.h"
#include "GT/Renderer/Model.h"
#include "GT/Renderer/Mesh.h"

namespace GT
{
	Scope<FileWatcher> AssetManager::s_FileWatcher;
	std::unordered_map<UUID, std::vector<std::function<void(uint32_t)>>> AssetManager::s_ReloadCallbacks;

	// From files's extension to get AssetType
	std::unordered_map<std::string, AssetType> AssetExt = {

		{".glsl",AssetType::Shader},
		{".comp",AssetType::ComputeShader},
		{".geom",AssetType::GeometryShader},

		{".png",AssetType::Texture2D},
		{".jpg",AssetType::Texture2D},

		{".dae",AssetType::Model},
		{".obj",AssetType::Model},

	};
	
	std::unordered_map<UUID, Ref<AssetInfo>> AssetManager::m_UUIDToAssetsInfo;
	std::unordered_map<std::filesystem::path, UUID> AssetManager::m_PathToUUID;
	std::unordered_multimap<std::string, UUID> AssetManager::m_NameToUUID;
	
	std::queue<uint32_t> AssetManager::AvailIndices;
	std::vector<AssetSlot> AssetManager::Assets;


	void AssetManager::WhenFileChanged(const std::filesystem::path& path, FileAction action)
	{
		if (std::filesystem::is_directory(path)) return;

		auto it = m_PathToUUID.find(path);

		if (it == m_PathToUUID.end())
		{
			GT_CORE_INFO("File {0} changed under asset dir and is not a registered asset.", path.string());
			return;
		}
		UUID id = it->second;
		Ref<AssetInfo> info;
		if (Existed(id, info))
		{
			switch (action)
			{
			case FileAction::Added:
				GT_CORE_INFO("File added: {0}", path.string());
				//if (s_ReloadCallbacks.count(id))
				//{
				//	for (auto& fn : s_ReloadCallbacks[id])
				//		fn();
				//}
				break;


			case FileAction::Removed:
				GT_CORE_WARN("File removed: {0}", path.string());
				break;


			case FileAction::Modified:
				GT_CORE_INFO("File modified: {0}", path.string());

				if (s_ReloadCallbacks.count(id))
				{
					for (auto& fn : s_ReloadCallbacks[id])
						fn(info->Index);
				}

				break;


			case FileAction::Renamed:
				GT_CORE_TRACE("File renamed: {0}", path.string());
				break;
			}
		}
	}

	void AssetManager::WatchFiles(const std::filesystem::path& path, std::function<void(const std::filesystem::path& path, FileAction action)> callback)
	{
		s_FileWatcher->Watch(path, callback);
	}

	void AssetManager::RegisterReloadCallback(const UUID& id, std::function<void(uint32_t)> callback)
	{
		s_ReloadCallbacks[id].push_back(callback);
	}

	void AssetManager::Init()
	{

		s_FileWatcher = CreateScope<FileWatcher>();
		s_FileWatcher->Start();

		WatchFiles("Resources");

		Assets.reserve(300);

		LoadInternalAssets();
	}

	void AssetManager::ShutDown()
	{
		Assets.clear();
	}
	Ref<AssetMetadata> AssetManager::LoadAssetMetadata(const std::filesystem::path& path)
	{
		auto json = Utils::LoadJSON(path);
		if (json.empty())
		{
			GT_CORE_ERROR("Can't load correctly of AssetsMetadata file {0}!", path.string());
			return nullptr;
		}

		Ref<AssetMetadata> meta = CreateRef<AssetMetadata>();

		meta->Name = json["Name"];
		meta->ID = uint64_t(json["ID"]);
		meta->Type = AssetTypeFromString(json["Type"]);
		meta->FilePath = std::filesystem::path(std::string(json["FilePath"]));
		meta->IsWatch = json["IsWatch"];

		return meta;
	}

	void AssetManager::LoadInternalAssets()
	{
		RegisterTexture2DAsset("Resources\\textures\\Checkerboard.png");
		RegisterTexture2DAsset("Resources\\Icons\\PlayButton.png");
		RegisterTexture2DAsset("Resources\\Icons\\StopButton.png");
		RegisterTexture2DAsset("Resources\\Icons\\SimulateButton.png");
		RegisterTexture2DAsset("Resources\\Icons\\DirectoryIcon.png");
		RegisterTexture2DAsset("Resources\\Icons\\FileIcon.png");
		RegisterTexture2DAsset("Resources\\Icons\\ModelIcon.png");
		RegisterTexture2DAsset("Resources\\Icons\\ShaderIcon.png");
		RegisterTexture2DAsset("Resources\\Icons\\TextureIcon.png");
		RegisterTexture2DAsset("Resources\\Icons\\SceneIcon.png");

		RegisterShaderAsset("Resources\\shaders\\Renderer2D_Quad.glsl");
		RegisterShaderAsset("Resources\\shaders\\Renderer2D_Circle.glsl");
		RegisterShaderAsset("Resources\\shaders\\Renderer2D_Line.glsl");
		RegisterShaderAsset("Resources\\shaders\\Model.glsl");
		RegisterShaderAsset("Resources\\shaders\\Particle.geom");
		RegisterShaderAsset("Resources\\shaders\\ShadowMap.glsl");
		RegisterShaderAsset("Resources\\shaders\\Renderer2D_UI.glsl");
		RegisterShaderAsset("Resources\\shaders\\Renderer2D_Text.glsl");

	}


	Handle AssetManager::GetAssetHandle(const std::string& name)
	{

		auto it = m_NameToUUID.find(name);
		if (it != m_NameToUUID.end())
		{
			UUID id = it->second;
			return GetAssetHandle(id);
		}
		else GT_CORE_WARN("Try to get asset from a not existed name->{0}.", name);

		return Handle();
	}

	Handle AssetManager::GetAssetHandle(const UUID& id)
	{
		Handle handle;

		if (m_UUIDToAssetsInfo.find(id) != m_UUIDToAssetsInfo.end())
		{
			auto it = m_UUIDToAssetsInfo.find(id);

			it->second->Refcount++;
			handle.index = it->second->Index;
			handle.ID = id;
			handle.generation = Assets[handle.index].generation;

		}
		else 
			GT_CORE_WARN("AssetManager::GetAssetHandle Try to get asset from a not existed uuid->{0}.", uint64_t(id));
		
		return handle;
	}

	const Ref<Asset> AssetManager::GetDefaultAsset(AssetType type)
	{
		UUID id;
		uint32_t index;
		switch (type)
		{
		case GT::AssetType::Scene:
			break;
		case GT::AssetType::Texture2D:
			return GetAsset("Checkerboard.png");
			break;
		case GT::AssetType::Texture3D:

			break;
		case GT::AssetType::Shader:

			break;
		case GT::AssetType::ComputeShader:

			break;
		case GT::AssetType::GeometryShader:

			break;
		case GT::AssetType::Model:

			break;
		}
		GT_CORE_ERROR("Unknow AssetType {0}!", AssetTypeToString(type));
		return nullptr;
	}


	void AssetManager::LoadDependenciesOfUUID(UUID id)
	{
	}

	const Ref<Asset> AssetManager::GetAsset(const Handle& handle)
	{
		if(Existed(handle.ID))
		{
			if (Assets[handle.index].asset->Info->NeedReload)
			{
				Ref<Asset> asset = ReloadAsset(handle);

				Assets[handle.index].asset->Info->NeedReload = false;
				asset->Info = Assets[handle.index].asset->Info;

				Assets[handle.index].asset = asset;
			}
			return Assets[handle.index].asset;
		}
		else
		{
			GT_CORE_ERROR("AssetManager::GetAsset: Invalid handle for asset with UUID {0}", uint64_t(handle.ID));
			return nullptr;
		}
	}

	const Ref<Asset> AssetManager::GetAsset(const std::string& name)
	{
		auto it = m_NameToUUID.find(name);

		if (it != m_NameToUUID.end())
			return GetAsset(it->second);
		
		GT_CORE_ERROR("AssetManager::GetAssetFromName name:{0} is not existed.", name);
		return Ref<Asset>();
	}

	const Ref<Asset> AssetManager::GetAsset(const UUID& id)
	{
		auto it = m_UUIDToAssetsInfo.find(id);

		if (it != m_UUIDToAssetsInfo.end())
		{
			Ref<AssetInfo> info = it->second;
			return Assets[info->Index].asset;
		}

		GT_CORE_ERROR("AssetManager::GetAssetFromUUID uuid:{0} is not existed.", uint64_t(id));
		return Ref<Asset>();
	}

	void AssetManager::LoadAssets(const Project& project)
	{
		auto paths = GetAllAssetPathsInDirectory(project.GetAssetDirectory());

		for(auto& path : paths)
		{
			
		}
	}


	bool AssetManager::Existed(const UUID& id, Ref<AssetInfo> info)
	{
		auto it = m_UUIDToAssetsInfo.find(id);
		if (it != m_UUIDToAssetsInfo.end())
		{
			info = it->second;
			return true;
		}
		info = nullptr;
		return false;
	}

	bool AssetManager::Existed(const Handle& handle, Ref<AssetInfo> info)
	{
		return Existed(handle.ID);
	}

	UUID AssetManager::RegisterAsset(const Ref<Asset> asset)
	{
		uint32_t index = GetNextAvialIndex();

		Ref<AssetInfo> info = m_UUIDToAssetsInfo.find(asset->ID)->second;
		AssetMetadata& meta = info->metadata;


		asset->Info = info;
		Assets[index].asset = asset;


		m_NameToUUID.emplace(meta.Name, meta.ID);
		m_PathToUUID.emplace(meta.FilePath, meta.ID);
		info->Index = index;



		RegisterReloadCallback(meta.ID, [](uint32_t index) {
			Assets[index].asset->Info->NeedReload = true;;
			});

		return meta.ID;
	}
	UUID AssetManager::RegisterShaderAsset(const std::filesystem::path& path)
	{
		auto it = m_PathToUUID.find(path);
		if (it != m_PathToUUID.end())
		{
			GT_CORE_WARN("AssetManager::RegisterShaderAsset asset {0} already registered.",path.string());
			return it->second;
		}

		auto asset = ShaderImporter::ImportShader(path);
		return RegisterAsset(asset);
	}
	UUID AssetManager::RegisterTexture2DAsset(const std::filesystem::path& path)
	{
		auto it = m_PathToUUID.find(path);
		if (it != m_PathToUUID.end())
		{
			GT_CORE_WARN("AssetManager::RegisterShaderAsset asset {0} already registered.", path.string());
			return it->second;
		}

		auto asset = TextureImporter::ImportTexture2D(path);
		return RegisterAsset(asset);
	}
	UUID AssetManager::RegisterTexture3DAsset(const std::filesystem::path& path)
	{
		auto it = m_PathToUUID.find(path);
		if (it != m_PathToUUID.end())
		{
			GT_CORE_WARN("AssetManager::RegisterShaderAsset asset {0} already registered.", path.string());
			return it->second;
		}
		return 0;
	}
	UUID AssetManager::RegisterModelAsset(const std::filesystem::path& path)
	{
		auto it = m_PathToUUID.find(path);
		if (it != m_PathToUUID.end())
		{
			GT_CORE_WARN("AssetManager::RegisterShaderAsset asset {0} already registered.", path.string());
			return it->second;
		}
		return 0;
	}
	UUID AssetManager::RegisterSceneAsset(const std::filesystem::path& path)
	{
		auto it = m_PathToUUID.find(path);
		if (it != m_PathToUUID.end())
		{
			GT_CORE_WARN("AssetManager::RegisterShaderAsset asset {0} already registered.", path.string());
			return it->second;
		}
		auto asset = SceneImporter::ImportScene(path);
		return RegisterAsset(asset);
	}

	//ShaderData& AssetManager::GetShaderAsset(const std::string& name)
	//{
	//	// TODO: insert return statement here
	//}
	//Texture2DData& AssetManager::GetTexture2DAsset(const std::string& name)
	//{
	//}
	//Texture3DData& AssetManager::GetTexture3DAsset(const std::string& name)
	//{
	//	// TODO: insert return statement here
	//}
	//ModelData& AssetManager::GetModelAsset(const std::string& name)
	//{
	//	// TODO: insert return statement here
	//}
	//SceneData& AssetManager::GetSceneAsset(const std::string& name)
	//{
	//	// TODO: insert return statement here
	//}



	void AssetManager::RegisterMetadata(AssetMetadata& meta)
	{
		Ref<AssetInfo> info = CreateRef<AssetInfo>();

		info->metadata = meta;
		m_UUIDToAssetsInfo.emplace(meta.ID, info);
	}

	void AssetManager::SaveMetadata(const Ref<AssetMetadata> Metadata)
	{
		//auto json = Utils::MetadataToJson(*Metadata);
		//auto jsonPath = Metadata->FilePath;
		//jsonPath.replace_extension(".meta");
		//Utils::SaveJSON(json, jsonPath);
	}

	UUID AssetManager::GetUUIDFromPath(const std::filesystem::path& path)
	{
		return m_PathToUUID.at(path);
	}
	const Ref<AssetInfo> AssetManager::GetAssetInfoFromUUID(const UUID& id)
	{
		return m_UUIDToAssetsInfo.at(id);
	}
	bool AssetManager::ReleaseHandle(const Handle& handle)
	{
		if (Existed(handle.ID))
		{
			auto& count = Assets[handle.index].asset->Info->Refcount;

			if (count == 0)
			{
				GT_CORE_ERROR("AssetManager::ReleaseHandle: Refcount for asset with UUID {0} is negative!", uint64_t(handle.ID));
				count = 1;
			}

			count--;
			//if (count == 0)
			//{
			//	// Unload asset
			//	Assets[handle.index].asset->Info->IsLoaded = false;
			//	Assets[handle.index].asset.reset();
			//	Assets[handle.index].asset = nullptr;
			//	Assets[handle.index].generation++;
			//	AvailIndices.push(handle.index);
			//}

		}
		return true;
	}


	// if next handle is a cycled one, ResourceTable erease unload asset
	uint32_t AssetManager::GetNextAvialIndex()
	{
		uint32_t slot;

		if (AvailIndices.size() > 0)
		{
			slot = AvailIndices.front();
			AvailIndices.pop();
		}
		else 
		{
			slot = Assets.size();
			Assets.emplace_back();
		}

		return slot;
	}

	
	std::vector<std::filesystem::path> AssetManager::GetAllAssetPathsInDirectory(const std::filesystem::path& directory)
	{
		std::vector<std::filesystem::path> paths;
		for (auto& p : std::filesystem::recursive_directory_iterator(directory))
		{
			if (std::filesystem::is_regular_file(p))
			{
				if (p.path().extension() == ".meta")
					paths.push_back(p.path());
			}
			if (std::filesystem::is_directory(p))
			{
				auto b = GetAllAssetPathsInDirectory(p.path());
				std::move(b.begin(), b.end(), std::back_inserter(paths));
			}
		}
		return paths;
	}


	std::vector<std::filesystem::path> AssetManager::GetAllItemPathsInDirectory(const std::filesystem::path& directory)
	{
		std::vector<std::filesystem::path> paths;
		for(auto& p : std::filesystem::recursive_directory_iterator(directory))
		{
			if (std::filesystem::is_regular_file(p))
			{
				paths.push_back(p.path());
			}
			if(std::filesystem::is_directory(p))
			{
				auto b = GetAllItemPathsInDirectory(p.path());
				std::move(b.begin(), b.end(), std::back_inserter(paths));
			}
		}
		return paths;
	}

	Ref<Asset> AssetManager::ReloadAsset(const Handle& handle)
	{
		auto info = GetAssetInfo(handle.ID);
		Ref<Asset> asset;
		switch (info->metadata.Type)
		{
		case GT::AssetType::Scene:
			break;
		case GT::AssetType::Texture2D:
			asset = TextureImporter::ReloadTexture2D(info->metadata.FilePath);
			break;
		case GT::AssetType::Texture3D:
			break;
		case GT::AssetType::Shader:
			asset = ShaderImporter::ReloadShader(info->metadata.FilePath);
			break;
		case GT::AssetType::ComputeShader:
			asset = ShaderImporter::ReloadShader(info->metadata.FilePath);
			break;
		case GT::AssetType::GeometryShader:
			asset = ShaderImporter::ReloadShader(info->metadata.FilePath);
			break;
		case GT::AssetType::Model:
			break;
		}

		asset->Name = info->metadata.Name;
		asset->ID = handle.ID;

		return asset;
	}

}