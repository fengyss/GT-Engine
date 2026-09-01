#pragma once

#include "GT/Core/Asset/Handle.h"
#include "GT/Core/Asset/ModelAsset.h"

namespace GT
{

	class Model
	{
	public:
		Model() = default;
		Model(const Model& tex);
		Model(const std::filesystem::path& path);
		~Model();



		Ref<ModelAsset> Get() const;

		void Reset(const Handle& hdl);

		Ref<ModelAsset> operator->()
		{
			return Get();
		}
		const Ref<ModelAsset> operator->() const
		{
			return Get();
		}

		Model& operator=(const Model& rhs);

		explicit operator bool() const  noexcept {
			return IsValid;
		}

		bool operator==(const Model& rhs) const
		{
			return handle.ID == rhs.handle.ID;
		}

	private:
		Handle handle;
		bool IsValid = false;
	};


}


