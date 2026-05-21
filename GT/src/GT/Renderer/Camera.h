#pragma once
#include "glm/glm.hpp"

namespace GT
{
	class Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };

	public:
		Camera() = default;
		Camera(const glm::mat4 projection)
			: m_Projection(projection) { }
		virtual ~Camera() = default;

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		virtual void SetProjectionType(ProjectionType type) { m_ProjectionType = type; }


		void SetViewMatrix(const glm::vec3& postion, const float& rotation);
		void SetViewMatrix(const glm::mat4& transform);

		virtual const glm::vec3& GetPosition() const { return m_Position; }

		const glm::mat4& GetProjection() const { return m_Projection; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix;; }

	protected:
		glm::vec3 m_Position = { 0.0f,0.0f,0.0f };
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_Projection = glm::mat4(1.0f);
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;
	};


}
