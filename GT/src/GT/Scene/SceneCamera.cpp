#include "gtpch.h"
#include "SceneCamera.h"

#include "glm/gtc/matrix_transform.hpp"

GT::SceneCamera::SceneCamera()
{

	RecalculateProjection();
}

void GT::SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
{
	m_ProjectionType = ProjectionType::Orthographic;
	m_OrthographicSize = size;
	m_OrthographicNear = nearClip;
	m_OrthographicFar = farClip;

	RecalculateProjection();
}
void GT::SceneCamera::SetPerspective(float verticalFov, float nearClip, float farClip)
{
	m_ProjectionType = ProjectionType::Perspective;
	m_PerspectiveFov = verticalFov;
	m_PerspectiveNear = nearClip;
	m_PerspectiveFar = farClip;
	RecalculateProjection();
}

void GT::SceneCamera::SetPerspective(float verticalFov, float aspectRatio, float nearClip, float farClip)
{
	m_ProjectionType = ProjectionType::Perspective;
	m_PerspectiveFov = verticalFov;
	m_PerspectiveNear = nearClip;
	m_PerspectiveFar = farClip;
	m_AspectRatio = aspectRatio;
	RecalculateProjection();
}


void GT::SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
{
	m_AspectRatio = (float)width / (float)height;

	m_ViewportWidth = width;
	m_ViewportHeight = height;
	RecalculateProjection();

}


void GT::SceneCamera::RecalculateProjection()
{
	switch (m_ProjectionType)
	{
	case GT::SceneCamera::ProjectionType::Perspective:
	{
		m_Projection = glm::perspective(m_PerspectiveFov, m_AspectRatio,
			m_PerspectiveNear, m_PerspectiveFar);
	}
		break;
	case GT::SceneCamera::ProjectionType::Orthographic:
	{
		float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthoBottom = -m_OrthographicSize * 0.5f;
		float orthoTop = m_OrthographicSize * 0.5f;

		m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop,
			m_OrthographicNear, m_OrthographicFar);
	}
		break;
	default:
		break;
	}

}
