#pragma once
#include "GT/Renderer/Camera.h"
namespace GT
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };

	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);


		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }

		// Orthographic Getters and Setters
		float GetOrthographicSize() const { return m_OrthographicSize; }
		float GetOrthographicNearClip() const { return m_OrthographicNear; }
		float GetOrthographicFarClip() const { return m_OrthographicFar; }

		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
		void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection();}
		void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection();}




		void SetPerspective(float verticalFov, float nearClip, float farClip);

		//Perspective Getters and Setters
		float GetPerspectiveVerticalFov() const { return m_PerspectiveFov; }
		float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		float GetPerspectiveFarClip() const { return m_PerspectiveFar; }

		void SetPerspectiveVerticalFov(float fov) { m_PerspectiveFov = fov; RecalculateProjection(); }
		void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
		void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar = 1.0f;

		float m_PerspectiveFov = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f;
		float m_PerspectiveFar = 1000.0f;


		float m_AspectRatio = 0.0f;
	};
}


