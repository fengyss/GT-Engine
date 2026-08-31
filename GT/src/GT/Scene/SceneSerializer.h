#pragma once

#include "Scene.h"

namespace GT
{
	class SceneSerializer
	{
	public:
		SceneSerializer() = default;
		void LoadFile(const std::filesystem::path& filepath);

		void Serialize(Ref<Scene> scene, const std::filesystem::path& filepath);
		void SerializeRuntime(const std::filesystem::path& filepath);

		Ref<Scene> Deserialize(const std::filesystem::path& filepath);
		bool DeserializeRuntime(const std::filesystem::path& filepath);


		// Based on scene file generate metadata
		Ref<AssetMetadata> GenerateMetadataFromFile(const std::filesystem::path& filepath);


	};
}


