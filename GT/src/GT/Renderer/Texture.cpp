#include "gtpch.h"
#include "Texture.h"

#include "GT/Renderer/Renderer.h"
#include "GT/Platform/OpenGL/OpenGLTexture.h"
#include "GT/Math/Math.h"

#include "GT/Core/Asset/Handle.h"
#include "GT/Core/Asset/AssetMetadata.h"
#include "GT/Core/Asset/AssetManager.h"

namespace GT
{
	Texture2D::Texture2D(const Texture2D& tex)
	{
		AssetManager::ReleaseHandle(handle);
		handle = AssetManager::GetAssetHandle(tex.handle.ID);
		IsValid = AssetManager::Existed(handle);

		GT_CORE_TRACE("Texture2D(const Texture2D&): {0}.", handle);
	}
	Texture2D::Texture2D(const std::filesystem::path& path)
	{
		if (path.has_extension())
		{
			handle.ID = AssetManager::RegisterTexture2DAsset(path);
			handle = AssetManager::GetAssetHandle(handle.ID);
		}
		else handle = AssetManager::GetAssetHandle(path.string());

		IsValid = AssetManager::Existed(handle);


		GT_CORE_TRACE("Texture2D({0}): {1}.",path, handle);
	}

	Texture2D::~Texture2D()
	{
		if(IsValid)
		{
			GT_CORE_TRACE("~Texture2D: {0}.", handle);
			AssetManager::ReleaseHandle(handle);
			IsValid = false;
		}
	}

	Ref<Texture2DAsset> Texture2D::Get() const
	{
		auto texture = AssetManager::GetAsset(handle);

		if (!texture)
		{
			GT_CORE_ERROR("Texture is unknown : {0}", uint64_t(handle.ID));
			texture = AssetManager::GetDefaultAsset(AssetType::Texture2D);
		}
		return std::dynamic_pointer_cast<Texture2DAsset>(texture);
	}

	Texture2D& Texture2D::operator=(const Texture2D& rhs)
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