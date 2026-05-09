#pragma once
#include <random>
namespace GT
{
	class Random
	{
	public:
		static void Init(uint32_t seed = 0);
		// return 0~1
		static float Float();
		// return -1~1
		static float NormalFloat();
		static int Int();
		static float Range(float l, float r);
		static glm::vec3 RangeVec3(float l, float r);
		static glm::vec3 InUnitSphere();
		static glm::vec3 OnUnitSphere();
		static glm::vec2 OnUnitCircle();
	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_real_distribution<float> s_ReadDistribution;
	};
}


