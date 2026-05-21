#include "gtpch.h"
#include "Camera.h"

namespace GT
{
	void Camera::SetViewMatrix(const glm::vec3& postion, const float& rotation)
	{
		m_ViewMatrix= glm::translate(glm::mat4(1.0f), postion) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));
		m_Position = postion;
	}
	void Camera::SetViewMatrix(const glm::mat4& transform)
	{
		m_ViewMatrix = glm::inverse(transform);
		m_Position = glm::vec3(transform[3][0], transform[3][1], transform[3][2]);
	}
}

