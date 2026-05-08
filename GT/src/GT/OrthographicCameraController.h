#pragma once
#include "GT/Renderer/OrthographicCamera.h"
#include "GT/Core/Timestep.h"

#include "GT/Events/ApplicationEvent.h"
#include "GT/Events/MouseEvent.h"

namespace GT
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; CalculateView(); }

		float GetAspectRatio() const { return m_AspectRatio; }

		void ResetRotation() { m_CameraRotation = 0.0f; m_Camera.SetRotation(m_CameraRotation); }

		void OnResize(float width, float height);

	private:
		void CalculateView();

		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;
		glm::vec4 m_Bounds;
		bool m_Rotation;
		glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 4.0f;
		float m_CameraRotationSpeed = 180.0f;
	};
}


