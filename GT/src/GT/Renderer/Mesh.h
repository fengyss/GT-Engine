#pragma once

#include "GT/Core/Asset/Handle.h"
#include "GT/Core/Asset/MeshAsset.h"

namespace GT
{

	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(const Mesh& tex);
		Mesh(const std::vector<Vertex>& _vertices, const std::vector<unsigned int>& _indices, const std::vector<Texture2D>& _textures);
		~Mesh();



		Ref<MeshAsset> Get() const;

		void Reset(const Handle& hdl);

		Ref<MeshAsset> operator->()
		{
			return Get();
		}
		const Ref<MeshAsset> operator->() const
		{
			return Get();
		}

		Mesh& operator=(const Mesh& rhs);

		explicit operator bool() const  noexcept {
			return IsValid;
		}

		bool operator==(const Mesh& rhs) const
		{
			return handle.ID == rhs.handle.ID;
		}

	private:
		Handle handle;
		bool IsValid = false;
	};


}


