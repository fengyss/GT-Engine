#pragma once

#include "Scene.h"

namespace GT
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(std::filesystem::path& filepath);
		void SerializeRuntime(std::filesystem::path& filepath);

		bool Deserialize(std::filesystem::path& filepath);
		bool DeserializeRuntime(std::filesystem::path& filepath);

	private:
		Ref<Scene> m_Scene;
	};
}


