#include "gtpch.h"
#include "AssetManager.h"
#include "AssetImporter.h"
#include "GT/Utils/JsonUtils.h"
#include "GT/Project/Project.h"
#include "GT/Math/Math.h"


namespace GT
{
	Scope<FileWatcher> AssetManager::s_FileWatcher;
	std::unordered_map<std::filesystem::path, std::vector<std::function<void(const std::filesystem::path&)>>> AssetManager::s_ReloadCallbacks;

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

	std::unordered_map<std::string, Ref<AssetMetadata>> MetaTable; // using name to retrive AssetMetadata
	std::unordered_map<std::string, Handle> ResourceTable; // using name to retrive AssetMetadata
	static std::unordered_multimap<std::filesystem::path, std::string> m_Paths; // use path to retrive metadata, and make sure not load same path

	std::vector<AssetSlot> AssetSlots;

	uint32_t TheLastSlot = 0; // refer the minimal index of Assets' unused slots
	std::vector<uint32_t> AvailSlots; // store cycled slots of Assets

	void AssetManager::WhenFileChanged(const std::filesystem::path& path, FileAction action)
	{
		switch (action)
		{
		case FileAction::Added:
			GT_CORE_INFO("File added: {0}", path.string());
			if (s_ReloadCallbacks.count(path))
			{
				for (auto& fn : s_ReloadCallbacks[path])
					fn(path);
			}
			break;


		case FileAction::Removed:
			GT_CORE_WARN("File removed: {0}", path.string());
			break;


		case FileAction::Modified:
			GT_CORE_INFO("File modified: {0}", path.string());
			if (s_ReloadCallbacks.count(path))
			{
				for (auto& fn : s_ReloadCallbacks[path])
					fn(path);
			}
			break;


		case FileAction::Renamed:
			GT_CORE_TRACE("File renamed: {0}", path.string());
			break;
		}
	}

	void AssetManager::WatchFiles(const std::filesystem::path& path, std::function<void(const std::filesystem::path& path, FileAction action)> callback)
	{
		s_FileWatcher->Watch(path, callback);
	}

	void AssetManager::RegisterReloadCallback(const std::filesystem::path& path, std::function<void(const std::filesystem::path&)> callback)
	{
		s_ReloadCallbacks[path].push_back(callback);
	}

	void AssetManager::Init()
	{

		s_FileWatcher = CreateScope<FileWatcher>();
		s_FileWatcher->Start();

		WatchFiles("Resources");

		AssetSlots.clear();
		AvailSlots.clear();

		MetaTable.clear();
		ResourceTable.clear();

		AssetSlots.reserve(1000);
		AvailSlots.reserve(1000);

		LoadInternalAssetsMetadata();
	}

	void AssetManager::ShutDown()
	{
		SaveAssetsMetadata();

		AssetSlots.clear();
		AvailSlots.clear();

		MetaTable.clear();
		ResourceTable.clear();
	}
	void AssetManager::LoadAssetsMetadata(const std::filesystem::path& path)
	{
		auto json = Utils::LoadJSON(path);
		if (json.empty())
		{
			GT_CORE_ERROR("Can't load correctly of AssetsMetadata file {0}!",path.string());
			return;
		}
		for (auto& asset : json["Assets"])
		{
			Ref<AssetMetadata> meta = CreateRef<AssetMetadata>();
			meta->FilePath = std::filesystem::path(std::string(asset["FilePath"]));
			meta->Name = asset["Name"];
			meta->Type = AssetTypeFromString(asset["Type"]);
			MetaTable.emplace(meta->Name,meta);
		}
	}
	void AssetManager::AddAssetMetadata(AssetMetadata meta)
	{
		GT_CORE_ASSERT(!meta.Name.empty(), "assetmeta name can't be empty!");
		if (MetaTable.find(meta.Name) != MetaTable.end())
		{
			GT_CORE_WARN("Metadata with name '{}' already exists, ignoring.", meta.Name);
		}

		//MetaTable[meta.Name] = std::make_shared<AssetMetadata>(meta);
		MetaTable.emplace(meta.Name, std::make_shared<AssetMetadata>(meta));

		m_Paths.insert({ meta.FilePath,meta.Name });
	}
	void AssetManager::SaveAssetsMetadata()
	{
		using json = nlohmann::json;

		json root;
		root["MetaDatas"] = json::array();

		for (auto& [name, meta] : MetaTable)
		{
			json data;
			data["Name"] = name;
			data["FilePath"] = meta->FilePath.string();
			data["UUID"] = uint64_t(meta->ID);
			data["Type"] = AssetTypeToString(meta->Type);
			root["MetaDatas"].push_back(data);
		}
		Utils::SaveJSON(root, Project::GetAssetDirectory() / "AssetsMetadata.json");
	}

	Ref<AssetMetadata> AssetManager::GetMetaFromName(const std::string& name)
	{

		if (MetaTable.find(name) != MetaTable.end()) return MetaTable.at(name);
		else NULL;
	}

	void AssetManager::DeleteMetaFromName(const std::string& name)
	{
		if (MetaTable.find(name) != MetaTable.end()) MetaTable.erase(name);
	}

	void AssetManager::DeleteMetaFromPath(const std::filesystem::path& path)
	{
		auto range = m_Paths.equal_range(path);
		for (auto it = range.first; it != range.second; ++it) {
			DeleteMetaFromName(it->second);
		}
		m_Paths.erase(path);
	}

	void AssetManager::LoadInternalAssetsMetadata()
	{

		AddAssetMetadata({ AssetType::Texture2D,"Default",UUID(), "Resources\\textures\\Checkerboard.png", true });

		AddAssetMetadata({ AssetType::Texture2D,"IconPlay",UUID(), "Resources\\Icons\\PlayButton.png", true });
		AddAssetMetadata({ AssetType::Texture2D,"IconStop",UUID(), "Resources\\Icons\\StopButton.png", true });
		AddAssetMetadata({ AssetType::Texture2D,"IconSimulate",UUID(), "Resources\\Icons\\SimulateButton.png", true });

		AddAssetMetadata({ AssetType::Texture2D,"DirectoryIcon",UUID(), "Resources\\Icons\\DirectoryIcon.png", true });
		AddAssetMetadata({ AssetType::Texture2D,"FileIcon",UUID(), "Resources\\Icons\\FileIcon.png", true });


		AddAssetMetadata({ AssetType::Shader,"Renderer2D_Quad",UUID(), "Resources\\shaders\\Renderer2D_Quad.glsl", true });
		AddAssetMetadata({ AssetType::Shader,"Renderer2D_Circle",UUID(), "Resources\\shaders\\Renderer2D_Circle.glsl" , true });
		AddAssetMetadata({ AssetType::Shader,"Renderer2D_Line",UUID(), "Resources\\shaders\\Renderer2D_Line.glsl" , true });
		AddAssetMetadata({ AssetType::Shader,"Model",UUID(), "Resources\\shaders\\Model.glsl" , true });
		AddAssetMetadata({ AssetType::GeometryShader,"Particles",UUID(), "Resources\\shaders\\Particle.geom" , true });
		AddAssetMetadata({ AssetType::Shader,"Shadow",UUID(), "Resources\\shaders\\ShadowMap.glsl" , true });
		AddAssetMetadata({ AssetType::Shader,"Renderer2D_UI",UUID(), "Resources\\shaders\\Renderer2D_UI.glsl" , true });
		AddAssetMetadata({ AssetType::Shader,"Renderer2D_Text",UUID(), "Resources\\shaders\\Renderer2D_Text.glsl", true });
	}

	Ref<Asset> AssetManager::GetDefaultAsset(AssetType type)
	{
		switch (type)
		{
		case AssetType::None:
			return nullptr;
		case AssetType::Texture2D:
			return nullptr;
		case AssetType::Scene:
			return nullptr;
		case AssetType::Shader:
			return nullptr;
		case AssetType::Texture3D:
			return nullptr;
		case AssetType::Model:
			return nullptr;

		}
		GT_CORE_ERROR("Unknow AssetType {0}!", AssetTypeToString(type));
		return nullptr;
	}
	// return false if failed
	// else return true  out will be this asset's handle
	bool AssetManager::LoadAsset(const std::string& name, Handle& out)
	{
		if (MetaTable.find(name) != MetaTable.end())
		{
			auto& meta = MetaTable.at(name);

			uint32_t slot = GetNextAvialHandle().slot;
			out = AssetSlots[slot].first;

			auto asset = AssetImporter::ImportAsset(*meta);
			if (!asset)
			{
				ReleaseAsset(out);
				GT_CORE_ERROR("Can't load asset:{0} from {1}!", meta->Name, meta->FilePath.string());
				return false;
			}
			asset->count = 1;
			AssetSlots[slot].second = asset;
			ResourceTable[name] = out;
			if(meta->IsWatch)
				AssetManager::s_ReloadCallbacks[meta->FilePath].push_back(ReloadAsset);
			return true;
		}
		else
		{
			GT_CORE_ERROR("Try to get a handle of {0}, while is not exited!", name);
			return false;
		}
	}
	// return Handle(), If asset not exited or can't load correctly
	Handle AssetManager::GetHandle(const std::string& name)
	{
		// if already loaded, get it from ResourceTable
		// else if it in MetaTable, load it
		if (ResourceTable.find(name) != ResourceTable.end())
		{
			auto& handle = ResourceTable.at(name);

			uint32_t slot = handle.slot;
			auto asset = AssetSlots[slot].second;
			asset->count++;

			return handle;
		}
		else
		{
			Handle handle;
			bool succeed = LoadAsset(name, handle);
			if (succeed) return handle;
			else return Handle();
		}
	}
	// if asset released succeedly return true
	// and will set handle.generation = 0, which means
	// this handle invalid
	bool AssetManager::ReleaseAsset(Handle& handle)
	{
		uint32_t slot = handle.slot;
		if (slot >= TheLastSlot) 
		{ 
			GT_CORE_ERROR("Try to release a asset handle which slot out of boundary!"); 
			return false; 
		}
		auto& count = AssetSlots[slot].second->count;
		if (count == 0)
		{
			GT_CORE_ERROR("Try to release a asset handle which asset already inactive(might released)!");
			return false;
		}

		count--;

		if (count == 0)
		{
			AvailSlots.push_back(slot);
		}
		handle.generation = 0;
		return true;
	}
	Ref<Asset> AssetManager::GetAsset(const Handle& handle)
	{
		uint32_t slot = handle.slot;
		if (slot >= TheLastSlot)
		{
			GT_CORE_ERROR("Try to get a asset which handle.slot out of boundary!");
			return nullptr;
		}
		auto& assetslot = AssetSlots[handle.slot];
		if (assetslot.first == handle)
		{
			assetslot.second->count++;
			if (assetslot.second->NeedReload) 
			{
				assetslot.second = AssetImporter::ImportAsset(assetslot.second->metadata);
			}
			return assetslot.second;
		}

		GT_CORE_ERROR("Try to Get asset from a not matched handle!");
		return GetDefaultAsset(assetslot.second->metadata.Type);
	}
	// if this is not given a name, it's name will be uuid
	Handle AssetManager::LoadAsset(const std::filesystem::path& path, const std::string& name, bool IsWatch)
	{
		// if loaded not load again
		if (m_Paths.find(path) != m_Paths.end())
		{
			std::string before = m_Paths.find(path)->second;
			if (name != before)
			{
				m_Paths.insert({ path,name });
				MetaTable[name] = GetMetaFromName(before);
			}
			return GetHandle(before);
		}


		Ref<AssetMetadata> meta = CreateRef<AssetMetadata>();

		std::string extension = path.extension().string();
		if (AssetExt.find(extension) != AssetExt.end()) meta->Type = AssetExt.at(extension);
		else return Handle();

		meta->FilePath = path;
		if (name.empty()) meta->Name = Math::U64ToString(meta->ID);

		auto asset = AssetImporter::ImportAsset(*meta);

		if (asset) asset->count = 1;
		else return Handle();

		Handle handle = GetNextAvialHandle();

		meta->IsWatch = IsWatch;
		ResourceTable[name] = handle;
		MetaTable[meta->Name] = meta;
		AssetSlots[handle.slot].second = asset;
		m_Paths.insert({ path,name });

		if (IsWatch) s_ReloadCallbacks[path].push_back(ReloadAsset);

		return handle;
	}
	// if next handle is a cycled one, ResourceTable erease unload asset
	Handle AssetManager::GetNextAvialHandle()
	{
		uint32_t slot = TheLastSlot;
		if (AvailSlots.size() > 0)
		{
			slot = *AvailSlots.cend();
			AvailSlots.pop_back();
			ResourceTable.erase(AssetSlots[slot].second->metadata.Name);
		}
		else 
		{
			AssetSlot slot;
			AssetSlots.push_back(slot);
			TheLastSlot++;
		}

		Handle& handle = AssetSlots[slot].first;
		handle.generation++;
		handle.slot = slot;

		return handle;
	}
	void AssetManager::ReloadAsset(const std::filesystem::path& path)
	{
		auto name = m_Paths.find(path)->second;
		auto handle = GetHandle(name);

		AssetSlots[handle.slot].second->NeedReload = true;;
	}
}