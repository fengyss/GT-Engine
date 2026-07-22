#pragma once
#include "Asset.h"
#include "AssetHandle.h"


#include "GT/Core/FileWatcher.h"

namespace GT
{
	using AssetSlot = std::pair<Handle, Ref<Asset>>;

	class AssetManager
	{
	public:
		static void Init();
		static void ShutDown();

		static void WhenFileChanged(const std::filesystem::path& path, FileAction action);
		static void WatchFiles(const std::filesystem::path& path, std::function<void(const std::filesystem::path& path, FileAction action)> callback = WhenFileChanged);

		// Registers a callback function that will be called when the asset at the given path is reloaded
		static void RegisterReloadCallback(const std::filesystem::path& path, std::function<void(const std::filesystem::path&)> callback);

		// load all assets metadata(uuid,name,type,filepath) and store them to ResourceTable
		static void LoadAssetsMetadata(const std::filesystem::path& path);
		static void AddAssetMetadata(AssetMetadata meta);
		static void SaveAssetsMetadata();

		static Ref<AssetMetadata> GetMetaFromName(const std::string& name);

		static void DeleteMetaFromName(const std::string& name);
		static void DeleteMetaFromPath(const std::filesystem::path& path);

		static Handle GetHandle(const std::string& name);
		static bool ReleaseAsset(Handle& handle);
		static Ref<Asset> GetAsset(const Handle& handle);
		static Handle LoadAsset(const std::filesystem::path& path, const std::string& name, bool IsWatch);

	private:
		static void LoadInternalAssetsMetadata();
		static Ref<Asset> GetDefaultAsset(AssetType type);
		static bool LoadAsset(const std::string& name, Handle& out);
		static Handle GetNextAvialHandle();

		static void ReloadAsset(const std::filesystem::path& path);
		
		static Scope<FileWatcher> s_FileWatcher;
		static std::unordered_map<std::filesystem::path, std::vector<std::function<void(const std::filesystem::path&)>>> s_ReloadCallbacks;

	};
}
