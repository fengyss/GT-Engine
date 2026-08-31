#include "gtpch.h"


#include "GT/Core/Asset/AssetMetadata.h"
#include "GT/Core/Asset/ShaderAsset.h"

#include "GT/Core/Asset/AssetManager.h"

#include "Shader.h"


namespace GT
{
	Shader::Shader(const Shader& shader)
	{
		if (shader.IsValid)
		{
			handle = AssetManager::GetAssetHandle(shader.handle.ID);
			IsValid = AssetManager::Existed(handle);

			GT_CORE_TRACE("Shader(const Shader&): {0}.", handle);
		}
		else
		{
			handle = Handle();
			IsValid = false;
		}

	}
	Shader::Shader(const std::filesystem::path& path)
	{
		if (path.has_extension())
		{
			handle.ID = AssetManager::RegisterTexture2DAsset(path);
			handle = AssetManager::GetAssetHandle(handle.ID);
		}
		else handle = AssetManager::GetAssetHandle(path.string());

		IsValid = AssetManager::Existed(handle);


		GT_CORE_TRACE("Shader({0}): {1}.", path, handle);
	}

	Shader::~Shader()
	{
		if (IsValid)
		{
			GT_CORE_TRACE("~Shader: {0}.", handle);
			AssetManager::ReleaseHandle(handle);
			IsValid = false;
		}
	}



	Ref<ShaderAsset> Shader::Get() const
	{
		auto shader = AssetManager::GetAsset(handle);

		if(!shader)
		{
			GT_CORE_ERROR("Shader is unknown : {0}", handle);
			shader = AssetManager::GetDefaultAsset(AssetType::Shader);
		}
		return std::dynamic_pointer_cast<ShaderAsset>(shader);
	}

	void Shader::Reset(const Handle& hdl)
	{
		handle = hdl;
		IsValid = AssetManager::Existed(handle);
	}

	Shader& Shader::operator=(const Shader& rhs)
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