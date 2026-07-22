#include "gtpch.h"
#include "Project.h"
#include "GT/Scripting/ScriptEngine.h"
#include "ProjectSerializer.h"
#include "GT/Core/Asset/AssetManager.h"
namespace GT {

	Ref<Project> Project::New()
	{
		s_ActiveProject = CreateRef<Project>();
		return s_ActiveProject;
	}

	Ref<Project> Project::Load(const std::filesystem::path& path)
	{
		Ref<Project> project = CreateRef<Project>();

		ProjectSerializer serializer(project);
		if (serializer.Deserialize(path))
		{
			project->m_ProjectDirectory = path.parent_path();
			s_ActiveProject = project;

			ScriptEngine::LoadAppAssembly(GetAssetFileSystemPath(project->GetConfig().ScriptModulePath));
			//AssetsManager::WatchFiles(GetAssetDirectory());

			return s_ActiveProject;
		}

		return nullptr;
	}

	bool Project::SaveActive(const std::filesystem::path& path)
	{
		ProjectSerializer serializer(s_ActiveProject);
		if (serializer.Serialize(path))
		{
			s_ActiveProject->m_ProjectDirectory = path.parent_path();
			return true;
		}

		return false;
	}

}
