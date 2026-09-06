#pragma once

#include "GT/Scene/SceneCamera.h"
#include "GT/Core/Timestep.h"
#include "GT/Events/Event.h"
#include "GT/Events/MouseEvent.h"
#include "GT/Events/KeyEvent.h"

namespace glm {
	typedef vec<2, float, defaultp>		vec2;
	typedef vec<3, float, defaultp>		vec3;
	typedef vec<4, float, defaultp>		vec4;
	typedef mat<4, 4, f32, defaultp>	mat4;
	typedef qua<float, defaultp>		quat;
}

namespace GT {

	class EditorCamera : public SceneCamera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);
		EditorCamera(float size, float nearClip, float farClip);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }
		



		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;

		virtual void SetProjectionType(ProjectionType type);
		void CancelDraging() { IsDraging = false; }

		virtual const glm::vec3& GetPosition() const;
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }

		void FlipMouseFollow();
	private:
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void RotateView(const glm::vec2& delta);
		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:


		bool IsDraging = false;
		float Orth_Speed = 20.0f;
		glm::vec2 Orth_DragPos = glm::vec2(0.0f);
		glm::vec3 Orth_Offets = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Orth_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Orth_FocalPoint = { 0.0f, 0.0f, 0.0f };


		glm::vec3 Pers_Offets = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Pers_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Pers_FocalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		float m_Distance = 1.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;


		bool IsEnableMouseFollow = false;
	};

}
