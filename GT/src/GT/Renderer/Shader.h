#pragma once

#include "GT/Core/Asset/Handle.h"
#include "GT/Core/Asset/ShaderAsset.h"

namespace GT
{

	class Shader
	{
	public:
		Shader() = default;
		Shader(const Shader& shader);
		Shader(const std::filesystem::path& path);
		~Shader();


		Ref<ShaderAsset> Get() const;

		void Reset(const Handle& hdl);
		Ref<ShaderAsset>operator->()
		{
			return Get();
		}

		const Ref<ShaderAsset> operator->() const
		{
			return Get();
		}

		explicit operator bool() const  noexcept {
			return IsValid;
		}

		bool operator==(const Shader& rhs) const
		{
			return handle.ID == rhs.handle.ID;
		}
		Shader& operator=(const Shader& rhs);

	private:
		Handle handle;
		bool IsValid = false;
	};
}