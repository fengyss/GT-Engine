#pragma once
#include "Asset.h"
#include "Handle.h"


#include "GT/Core/FileWatcher.h"

namespace GT
{
	class Project;
	// asset pointer and generation
	struct AssetSlot
	{
		Ref<Asset> asset;
		uint32_t generation;
	};


	class AssetManager
	{
	public:
		static void Init();
		static void ShutDown();

		static void WhenFileChanged(const std::filesystem::path& path, FileAction action);
		static void WatchFiles(const std::filesystem::path& path, std::function<void(const std::filesystem::path& path, FileAction action)> callback = WhenFileChanged);

		// Registers a callback function that will be called when the asset at the given path is reloaded
		static void RegisterReloadCallback(const UUID& id, std::function<void(uint32_t)> callback);



		static const std::vector<AssetSlot>& GetAssets() { return Assets; }
		static const std::queue<uint32_t>& GetAvailIndices() { return AvailIndices; }

		// load all assets in the project, and register them to AssetManager
		static void LoadAssets(const Project& project);
		// load asset metadata from the given path, and register them to AssetManager
		static Ref<AssetMetadata> LoadAssetMetadata(const std::filesystem::path& path);

		// If handle is valid or handle'uuid is exist,
		// return true and make sure handle is valid.
		// else return false and handle will remain unchanged.

		static bool Existed(const UUID& id, Ref<AssetInfo> info = nullptr);
		static bool Existed(const Handle& handle, Ref<AssetInfo> info = nullptr);

		// load asset from the given path, and register them to AssetManager
		// generator a metadata for the asset if not exist, and save it to the same directory of the asset file.
		// return a handle to the asset
		static UUID RegisterAsset(const Ref<Asset> asset);

		static UUID RegisterShaderAsset(const std::filesystem::path& path);
		static UUID RegisterTexture2DAsset(const std::filesystem::path& path);
		static UUID RegisterTexture3DAsset(const std::filesystem::path& path);
		static UUID RegisterModelAsset(const std::filesystem::path& path);
		static UUID RegisterSceneAsset(const std::filesystem::path& path);


		
		// save asset metadata to file at FilePath
		static void SaveMetadata(const Ref<AssetMetadata> Metadata);

		static UUID GetUUIDFromPath(const std::filesystem::path& path);
		static const Ref<AssetInfo> GetAssetInfoFromUUID(const UUID& id);
		


		static bool ExistedUUID(const UUID& id) { return m_UUIDToAssetsInfo.find(id) != m_UUIDToAssetsInfo.end(); }



		static const Ref<Asset> GetAsset(const Handle& handle);
		static const Ref<Asset> GetAsset(const std::string& name);
		static const Ref<Asset> GetAsset(const UUID& id);


		static Handle GetAssetHandle(const std::string& name);
		static Handle GetAssetHandle(const UUID& id);
		static bool ReleaseHandle(const Handle& handle);


		static const Ref<Asset> GetDefaultAsset(AssetType type);

		static void RegisterMetadata(AssetMetadata& meta);

		// generator a normal based on path
		static Ref<AssetMetadata> GeneratorMetadataFromPath(const std::filesystem::path& path);

		// recursively load all dependent assets
		static void LoadDependenciesOfUUID(UUID id);

	private:


		static const AssetMetadata& GetAssetMetadata(const UUID& id) { return m_UUIDToAssetsInfo.at(id)->metadata; }
		static const Ref<AssetInfo>& GetAssetInfo(const UUID& id) { return m_UUIDToAssetsInfo.at(id); }

		static void LoadInternalAssets();

		static uint32_t GetNextAvialIndex();

		// only return meta file path (relative path)
		static std::vector<std::filesystem::path> GetAllAssetPathsInDirectory(const std::filesystem::path& directory);


		// return all file path (relative path)
		static std::vector<std::filesystem::path> GetAllItemPathsInDirectory(const std::filesystem::path& directory);

		static Ref<Asset> ReloadAsset(const Handle& handle);
		
		static Scope<FileWatcher> s_FileWatcher;
		static std::unordered_map<UUID, std::vector<std::function<void(uint32_t)>>> s_ReloadCallbacks;

	private:


		static std::unordered_map<UUID, Ref<AssetInfo>> m_UUIDToAssetsInfo; // using uuid to retrive  asset's index in AssetSlots and  AssetMetadata
		static std::unordered_map<std::filesystem::path, UUID> m_PathToUUID; // using path to retrive UUID
		static std::unordered_multimap<std::string, UUID> m_NameToUUID; // using name to retrive UUID


		static std::queue<uint32_t> AvailIndices; // store cycled index of Assets
		static std::vector<AssetSlot> Assets;
	};
}
