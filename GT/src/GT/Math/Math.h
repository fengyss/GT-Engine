#pragma once

#include <glm/glm.hpp>
#include<string>
#include<iostream>

namespace GT {

	
	namespace Math
	{

		bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);

		std::string U64ToString(uint64_t num);

		uint32_t fnv1a(const char* str);
		uint32_t murmur3_32(const char* key, size_t len, uint32_t seed = 0);
	}
	
}