#pragma once
#include "GT/Core/FileWatcher.h"
#include "GT/Math/Math.h"
#include "GT/Renderer/Shader.h"
#include "GT/Renderer/Texture.h"
namespace GT
{
	class Model;
	class ModelLibrary;

	class AssetsManager
	{
		public:
		AssetsManager() = default;
		~AssetsManager() = default;

		static void Init();
		static void ShutDown();

		// Loads an asset of type T from the given path. If the asset is already loaded, it returns a reference to the existing asset instead of loading it again.
		template<typename T>
		static Ref<T> Load(const std::filesystem::path& path)
		{
			if constexpr (std::is_same_v<T, Texture2D>)
			{
				return LoadTexture(path);
			}

			if constexpr (std::is_same_v<T, Shader>)
			{
				return LoadShader(path);
			}

			if constexpr (std::is_same_v<T, Model>)
			{
				return LoadModel(path);
			}

			GT_CORE_ASSERT(false, "Unsupported asset type");
			return nullptr;
		}

		// Reloads the asset at the given path and calls all registered callbacks for that path. This is useful for hot-reloading assets during development.
		template<typename T>
		static Ref<T> Reload(const std::filesystem::path& path)
		{
			if constexpr (std::is_same_v<T, Texture2D>)
				return ReloadTexture(path);

			if constexpr (std::is_same_v<T, Shader>)
				return ReloadShader(path);

			if constexpr (std::is_same_v<T, Model>)
				return ReloadModel(path);

			GT_CORE_ASSERT(false, "Unsupported asset type");
			return Ref<T>();
		}

		// Registers a callback function that will be called when the asset at the given path is reloaded
		static void RegisterReloadCallback(const std::filesystem::path& path, std::function<void()> callback);


		// Returns a handle to the asset, which can be used to get the asset and will automatically update when the asset is reloaded
		template<typename T>
		static Ref<T> Get(const std::string& name)
		{
			if constexpr (std::is_same_v<T, Texture2D>)
				return GetTexture(name);
			if constexpr (std::is_same_v<T, Shader>)
				return GetShader(name);
			if constexpr (std::is_same_v<T, Model>)
				return GetModel(name);
			GT_CORE_ASSERT(false, "Unsupported asset type");
			return nullptr;
		}

		template<typename T>
		static std::filesystem::path& GetPath(const std::string& name)
		{
			if constexpr (std::is_same_v<T, Texture2D>)
				return m_Paths[m_TexturesCache[name]];
			if constexpr (std::is_same_v<T, Shader>)
				return m_Paths[m_ShadersCache[name]];
			if constexpr (std::is_same_v<T, Model>)
				return m_Paths[m_ModelsCache[name]];
			GT_CORE_ASSERT(false, "Unsupported asset type");
			return std::filesystem::path();
		}

	public:
		static Ref<Texture2D> LoadTexture(const std::filesystem::path& path);
		static Ref<Shader> LoadShader(const std::filesystem::path& path);
		static Ref<Model> LoadModel(const std::filesystem::path& path);

		static Ref<Texture2D> LoadTexture(const std::string& name, const std::filesystem::path& path);
		static Ref<Shader> LoadShader(const std::string& name, const std::filesystem::path& path);
		static Ref<Model> LoadModel(const std::string& name, const std::filesystem::path& path);

		static Ref<Texture2D> ReloadTexture(const std::filesystem::path& path);
		static Ref<Shader> ReloadShader(const std::filesystem::path& path);
		static Ref<Model> ReloadModel(const std::filesystem::path& path);

		static Ref<Texture2D> GetTexture(const std::string& name);
		static Ref<Shader> GetShader(const std::string& name);
		static Ref<Model> GetModel(const std::string& name);

	private:

		static Scope<FileWatcher> s_FileWatcher;


		static std::unordered_map<uint32_t, std::filesystem::path> m_Paths;

		static TextureLibrary m_TextureLibrary;
		static std::unordered_map<std::string, uint32_t> m_TexturesCache;


		//static AssetLibrary m_AssetLibrary;
		//static std::unordered_map<std::string, uint32_t> m_AssetsCache;


		static ModelLibrary m_ModelLibrary;
		static std::unordered_map<std::string, uint32_t> m_ModelsCache;

		static ShaderLibrary m_ShaderLibrary;
		static std::unordered_map<std::string, uint32_t> m_ShadersCache;

		static std::unordered_map<std::filesystem::path, std::vector<std::function<void()>>> s_ReloadCallbacks;
	};

}


