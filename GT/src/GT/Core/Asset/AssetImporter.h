#pragma once
#include "Asset.h"
namespace GT

{
	class AssetImporter
	{
	public:
		static Ref<Asset> ImportAsset(const AssetMetadata& metadata);
	};
}
