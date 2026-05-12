#include "gtpch.h"
#include <filesystem>

#include "AssetsManager.h"
#include "GT/Renderer/Model.h"
namespace GT
{
	Scope<FileWatcher> AssetsManager::s_FileWatcher;

    std::unordered_map<uint32_t, std::filesystem::path> AssetsManager::m_Paths;

    TextureLibrary AssetsManager::m_TextureLibrary;
    std::unordered_map<std::string, uint32_t> AssetsManager::m_TexturesCache;

    ShaderLibrary AssetsManager::m_ShaderLibrary;
    std::unordered_map<std::string, uint32_t> AssetsManager::m_ShadersCache;


    ModelLibrary AssetsManager::m_ModelLibrary;
    std::unordered_map<std::string, uint32_t> AssetsManager::m_ModelsCache;

    std::unordered_map<std::filesystem::path, std::vector<std::function<void()>>> AssetsManager::s_ReloadCallbacks;

    void AssetsManager::Init()
    {
		s_FileWatcher = CreateScope<FileWatcher>(); 
        s_FileWatcher->Start();

        // ¼à¿Ø assets Ä¿Â¼
        s_FileWatcher->Watch("project\\assets", [](const std::filesystem::path& path, FileAction action) {
            switch (action)
            {
            case FileAction::Added:
                GT_CORE_INFO("File added: {0}", path.string());
                if (s_ReloadCallbacks.count(path))
                {
                    for (auto& fn : s_ReloadCallbacks[path])
                        fn();
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
                        fn();
                }
                break;


            case FileAction::Renamed:
                GT_CORE_TRACE("File renamed: {0}", path.string());
                break;
            }
            });

        LoadTexture("default","project\\assets\\textures\\Checkerboard.png");
        LoadTexture("IconPlay", "Resources\\Icons\\PlayButton.png");
        LoadTexture("IconStop", "Resources\\Icons\\StopButton.png");
        LoadTexture("IconSimulate", "Resources\\Icons\\SimulateButton.png");

        LoadShader("Renderer2D_Quad", "project\\assets\\shaders\\Renderer2D_Quad.glsl");
        LoadShader("Renderer2D_Circle", "project\\assets\\shaders\\Renderer2D_Circle.glsl");
        LoadShader("Renderer2D_Line", "project\\assets\\shaders\\Renderer2D_Line.glsl");
        LoadShader("Model", "project//assets\\shaders\\Model.glsl");

        LoadTexture("DirectoryIcon", "Resources\\Icons\\DirectoryIcon.png");
        LoadTexture("FileIcon", "Resources\\Icons\\FileIcon.png");
	}
    void AssetsManager::ShutDown()
    {
        m_ShadersCache.clear();
		m_TexturesCache.clear();

		m_TextureLibrary.Clear();
		m_ShaderLibrary.Clear();
    }
    Ref<Texture2D> AssetsManager::LoadTexture(const std::filesystem::path& path)
    {
        uint32_t ID = Math::fnv1a(path.string().c_str());
		auto tex = m_TextureLibrary.Load(ID, path);
		m_TexturesCache.emplace(tex->GetName(), ID);
        m_Paths.emplace(ID, path);
        return tex;
    }
    Ref<Texture2D> AssetsManager::LoadTexture(const std::string& name,const std::filesystem::path& path)
    {
        uint32_t ID = Math::fnv1a(path.string().c_str());
        auto tex = m_TextureLibrary.Load(ID, path);
        m_TexturesCache.emplace(name, ID);
        m_Paths.emplace(ID, path);
        return tex;
    }

    Ref<Texture2D> AssetsManager::ReloadTexture(const std::filesystem::path& path)
    {
        uint32_t ID = Math::fnv1a(path.string().c_str());
		return m_TextureLibrary.Reload(ID, path);
    }

    Ref<Shader> AssetsManager::LoadShader(const std::filesystem::path& path)
    {
        uint32_t ID = Math::fnv1a(path.string().c_str());
        auto shader = m_ShaderLibrary.Load(ID, path);
        m_ShadersCache.emplace(shader->GetName(), ID);
        m_Paths.emplace(ID, path);
        return shader;
    }
    Ref<Model> AssetsManager::LoadModel(const std::filesystem::path& path)
    {
        uint32_t ID = Math::fnv1a(path.string().c_str());
        auto model = m_ModelLibrary.Load(ID, path);
        m_ModelsCache.emplace(model->GetName(), ID);
        m_Paths.emplace(ID, path);
        return model;
    }
    Ref<Shader> AssetsManager::LoadShader(const std::string& name, const std::filesystem::path& path)
    {
        uint32_t ID = Math::fnv1a(path.string().c_str());
        auto shader = m_ShaderLibrary.Load(ID, path);
        m_ShadersCache.emplace(name, ID);
        m_Paths.emplace(ID, path);
        return shader;
    }

    Ref<Model> AssetsManager::LoadModel(const std::string& name, const std::filesystem::path& path)
    {
        uint32_t ID = Math::fnv1a(path.string().c_str());
        auto model = m_ModelLibrary.Load(ID, path);
        m_ShadersCache.emplace(name, ID);
        m_Paths.emplace(ID, path);
        return model;
    }

    Ref<Shader> AssetsManager::ReloadShader(const std::filesystem::path& path)
    {
        uint32_t ID = Math::fnv1a(path.string().c_str());
		return m_ShaderLibrary.Reload(ID, path);
    }

    Ref<Model> AssetsManager::ReloadModel(const std::filesystem::path& path)
    {
        uint32_t ID = Math::fnv1a(path.string().c_str());
        return m_ModelLibrary.Reload(ID, path);
    }

    Ref<Shader> AssetsManager::GetShader(const std::string& name)
    {
        if (m_ShadersCache.find(name) != m_ShadersCache.end())
        {
            uint32_t ID = m_ShadersCache[name];
            return m_ShaderLibrary.Get(ID);
        }
        else
        {
            GT_CORE_ERROR("Shader with name {0} not found!", name);
            return nullptr;
        }
    }

    Ref<Model> AssetsManager::GetModel(const std::string& name)
    {
        if (m_ShadersCache.find(name) != m_ShadersCache.end())
        {
            uint32_t ID = m_ModelsCache[name];
            return m_ModelLibrary.Get(ID);
        }
        else
        {
            GT_CORE_ERROR("Shader with name {0} not found!", name);
            return nullptr;
        }
    }


    Ref<Texture2D> AssetsManager::GetTexture(const std::string& name)
    {
        if (m_TexturesCache.find(name) != m_TexturesCache.end())
        {
            uint32_t ID = m_TexturesCache[name];
            return m_TextureLibrary.Get(ID);
        }
        else
        {
            GT_CORE_ERROR("Texture with name {0} not found!", name);
            return nullptr;
        }
    }

    
    void AssetsManager::RegisterReloadCallback(const std::filesystem::path& path, std::function<void()> callback)
    {
        s_ReloadCallbacks[path].push_back(callback);
    }




    

}


