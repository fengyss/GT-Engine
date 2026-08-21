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
		static void CopyHandle(Handle& handle);
		static Ref<Asset> GetAsset(const Handle& handle);
		static Handle LoadAsset(const std::filesystem::path& path, const std::string& name, bool IsWatch);


		static Ref<Asset> GetDefaultAsset(AssetType type);




		static const std::vector<AssetSlot>& GetAssetSlots() { return AssetSlots; }
		static const std::unordered_map<std::string, Ref<AssetMetadata>>& GetMetaTable() { return MetaTable; }
		static const std::unordered_map<std::string, Handle>& GetResourceTable() { return ResourceTable; }
		static const std::unordered_multimap<std::filesystem::path, std::string>& GetPathTable() { return m_Paths; }
		static const std::vector<uint32_t>& GetAvailSlots() { return AvailSlots; }

	private:
		static void LoadInternalAssetsMetadata();
		static bool LoadAsset(const std::string& name, Handle& out);
		static Handle GetNextAvialHandle();

		static void ReloadAsset(const std::filesystem::path& path);
		
		static Scope<FileWatcher> s_FileWatcher;
		static std::unordered_map<std::filesystem::path, std::vector<std::function<void(const std::filesystem::path&)>>> s_ReloadCallbacks;

	private:

		static std::unordered_map<std::string, Ref<AssetMetadata>> MetaTable; // using name to retrive AssetMetadata
		static std::unordered_map<std::string, Handle> ResourceTable; // using name to retrive AssetMetadata
		static std::unordered_multimap<std::filesystem::path, std::string> m_Paths; // use path to retrive metadata, and make sure not load same path

		static std::vector<AssetSlot> AssetSlots;

		static uint32_t TheLastSlot; // refer the minimal index of Assets' unused slots
		static std::vector<uint32_t> AvailSlots; // store cycled slots of Assets

	};
}
