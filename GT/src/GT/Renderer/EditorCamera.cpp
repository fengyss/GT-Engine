#include "gtpch.h"
#include "EditorCamera.h"

#include "GT/Core/Application.h"

#include "GT/Core/Input.h"
#include "GT/Core/KeyCodes.h"
#include "GT/Core/MouseButtonCodes.h"

//#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace GT {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
	{
		SetPerspective(fov, aspectRatio, nearClip, farClip);
	}

	EditorCamera::EditorCamera(float size, float nearClip, float farClip)
	{
		SetOrthographic(size, nearClip, farClip);
	}

	glm::vec3 position;
	void EditorCamera::UpdateView()
	{
		// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
		
		switch (m_ProjectionType)
		{
		case ProjectionType::Perspective:
			Pers_Position = CalculatePosition();
			Pers_Position += Pers_Offets;
			position = Pers_Position;
			break;
		case ProjectionType::Orthographic:
			Orth_Position = CalculatePosition();
			Orth_Position += Orth_Offets;
			position = Orth_Position;
			break;
		}

		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}


	const glm::vec3& EditorCamera::GetPosition() const
	{
		switch (m_ProjectionType)
		{
		case ProjectionType::Perspective:
			return Pers_Position;
			break;
		case ProjectionType::Orthographic:
			return Orth_Position;
			break;
		}
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	int EnableInfinite = 0;



	void EditorCamera::OnUpdate(Timestep ts)
	{
		const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
		glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
		m_InitialMousePosition = mouse;

		if (Input::IsKeyPressed(Key::LeftAlt))
		{
			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
				MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
				MouseZoom(delta.y);
		}

		if(IsEnableMouseFollow) RotateView(delta);
		

		if (Input::IsKeyPressed(Key::J))
			Application::Get().GetWindow().SetCursorClip(1);
		if (Input::IsKeyPressed(Key::K))
			Application::Get().GetWindow().SetCursorClip(2);
		if (Input::IsKeyPressed(Key::P))
			EnableInfinite ^= 1;

		float speed = 40.f;

		switch (m_ProjectionType)
		{
		case ProjectionType::Perspective:
			if (Input::IsKeyPressed(Key::W))
				Pers_Offets += (GetForwardDirection() * float(ts) * speed);
			if (Input::IsKeyPressed(Key::S))
				Pers_Offets -= GetForwardDirection() * float(ts) * speed;
			if (Input::IsKeyPressed(Key::A))
				Pers_Offets -= GetRightDirection() * float(ts) * speed;
			if (Input::IsKeyPressed(Key::D))
				Pers_Offets += GetRightDirection() * float(ts) * speed;
			break;
		case ProjectionType::Orthographic:
			if (Input::IsKeyPressed(Key::W))
				Orth_Offets += (GetUpDirection() * float(ts) * speed);
			if (Input::IsKeyPressed(Key::S))
				Orth_Offets -= GetUpDirection() * float(ts) * speed;
			if (Input::IsKeyPressed(Key::A))
				Orth_Offets -= GetRightDirection() * float(ts) * speed;
			if (Input::IsKeyPressed(Key::D))
				Orth_Offets += GetRightDirection() * float(ts) * speed;
			break;
		}
		


		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(GT_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	void EditorCamera::RotateView(const glm::vec2& delta)
	{
		const float rotationSpeed = 0.3f;
		m_Yaw += delta.x * rotationSpeed;
		m_Pitch += delta.y * rotationSpeed;
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		switch (m_ProjectionType)
		{
		case ProjectionType::Perspective:
			Pers_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
			Pers_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
			break;
		case ProjectionType::Orthographic:
			Orth_Offets += glm::vec3(delta * glm::vec2(xSpeed, ySpeed),0.0f);
			break;
		}
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		switch (m_ProjectionType)
		{
		case ProjectionType::Perspective:
		{
			float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
			m_Yaw += yawSign * delta.x * RotationSpeed();
			m_Pitch += delta.y * RotationSpeed();
		}
			break;
		case ProjectionType::Orthographic:
			break;
		}
	}

	void EditorCamera::MouseZoom(float delta)
	{
		switch (m_ProjectionType)
		{
		case ProjectionType::Perspective:
			m_Distance -= delta * ZoomSpeed();
			if (m_Distance < 1.0f)
			{
				Pers_FocalPoint += GetForwardDirection();
				m_Distance = 1.0f;
			}
			break;
		case ProjectionType::Orthographic:
			break;
		}
		
	}

	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	void EditorCamera::ChangeProjectionType(ProjectionType type)
	{
		SetProjectionType(type);
		switch (type)
		{
		case ProjectionType::Perspective:
			break;
		case ProjectionType::Orthographic:
			break;
		}
		UpdateView();
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		switch (m_ProjectionType)
		{
		case ProjectionType::Perspective:
			return Pers_FocalPoint - GetForwardDirection() * m_Distance;
			break;
		case ProjectionType::Orthographic:
			return Orth_FocalPoint - GetForwardDirection() * m_Distance;
			break;
		}
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		switch (m_ProjectionType)
		{
		case ProjectionType::Perspective:
			return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
			break;
		case ProjectionType::Orthographic:
			return glm::quat(glm::vec3(0.0f));
			break;
		}
	}

}
