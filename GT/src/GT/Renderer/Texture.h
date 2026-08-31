#pragma once

#include "GT/Core/Asset/Handle.h"
#include "GT/Core/Asset/TextureAsset.h"

namespace GT
{

	class Texture2D
	{
	public:
		Texture2D() = default;
		Texture2D(const Texture2D& tex);
		Texture2D(const std::filesystem::path& path);
		~Texture2D();



		Ref<Texture2DAsset> Get() const;


		Ref<Texture2DAsset> operator->() 
		{ 
			return Get();
		}
		const Ref<Texture2DAsset> operator->() const
		{ 
			return Get();
		}

		Texture2D& operator=(const Texture2D& rhs);

		explicit operator bool() const  noexcept {
			return IsValid;
		}

		bool operator==(const Texture2D& rhs) const
		{
			return handle.ID == rhs.handle.ID;
		}

	private:
		Handle handle;
		bool IsValid = false;
	};


}


