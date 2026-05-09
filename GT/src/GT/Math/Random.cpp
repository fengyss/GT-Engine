#include "gtpch.h"
#include "Random.h"

namespace GT
{
	std::mt19937 Random::s_RandomEngine;
	std::uniform_real_distribution<float> Random::s_ReadDistribution;

	void Random::Init(uint32_t seed) {
		if (seed == 0)
			s_RandomEngine.seed(std::random_device()());
		else
			s_RandomEngine.seed(seed);
	}
	// return 0~1
	float Random::Float()
	{
		float k = (float)s_ReadDistribution(s_RandomEngine);
		//GT_CORE_INFO("Random Float: {0}", k);
		return k;
	}
	// return -1~1
	float Random::NormalFloat()
	{
		return Float() * 2.0 - 1.0;
	}
	int Random::Int()
	{
		return (float)s_ReadDistribution(s_RandomEngine);
	}
	float Random::Range(float l, float r)
	{
		return Float() * (r - l) + l;
	}
	glm::vec3 Random::RangeVec3(float l, float r)
	{
		return glm::vec3(
			Float() * (r - l) + l,
			Float() * (r - l) + l,
			Float() * (r - l) + l
		);
	}
	glm::vec3 Random::InUnitSphere()
	{
		float theta = Range(0.0f, glm::two_pi<float>());
		float phi = acos(NormalFloat());
		float r = cbrt(NormalFloat());

		float x = r * sin(phi) * cos(theta);
		float y = r * sin(phi) * sin(theta);
		float z = r * cos(phi);

		return glm::normalize(glm::vec3(x, y, z));
	}
	glm::vec3 Random::OnUnitSphere()
	{

		float theta = Range(0.0f, glm::two_pi<float>());
		float phi = acos(NormalFloat());

		float x = sin(phi) * cos(theta);
		float y = sin(phi) * sin(theta);
		float z = cos(phi);

		return glm::normalize(glm::vec3(x, y, z));
	}
	glm::vec2 Random::OnUnitCircle()
	{
		float angle = Range(0.0f, glm::two_pi<float>());

		float x = cos(angle);
		float y = sin(angle);
		return glm::vec2(x, y);
	}
}