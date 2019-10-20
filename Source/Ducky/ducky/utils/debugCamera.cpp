#include "ducky_pch.h"
#include "debugCamera.h"
#include "graphics\window.h"

namespace ducky { namespace utils {

	DebugCamera::DebugCamera(const glm::mat4& projection) :
		Camera(projection)
	{
		UpdateTransform();
		UpdateViewMatrix();
	}

	void DebugCamera::Update()
	{
		graphics::Input* input = graphics::Window::Instance->GetInput();
		glm::vec2 delta = input->GetMouseDelta();
		if (input->IsKeyDown(SDL_SCANCODE_LALT))
		{
			bool ur = false;
			if (input->IsMouseButtonDown(1))
			{
				Rotate(delta);
				ur = true;
			}
			else if (input->IsMouseButtonDown(2))
			{
				Pan(delta);
				ur = true;
			}
			else if (input->IsMouseButtonDown(3))
			{
				Zoom(delta);
				ur = true;
			}

			if (ur)
			{
				UpdateTransform();
				UpdateViewMatrix();
			}
		}
	}

	void DebugCamera::Zoom(const glm::vec2& delta)
	{
		m_Distance -= delta.y * m_ZoomSpeed;
		if (m_Distance < 0.1f)
			m_Distance = 0.1f;
	}

	void DebugCamera::Rotate(const glm::vec2& delta)
	{
		m_Yaw -= delta.x * m_RotationSpeed;
		m_Pitch += delta.y * m_RotationSpeed;
		if (m_Pitch < -M_PI / 2.0f + 0.001f)
			m_Pitch = -M_PI / 2.0f + 0.001f;
		if (m_Pitch > M_PI / 2.0f - 0.001f)
			m_Pitch = M_PI / 2.0f - 0.001f;
	}

	void DebugCamera::Pan(const glm::vec2& delta)
	{
		glm::vec3 forward = -glm::normalize(glm::vec3(
			glm::sin(m_Yaw) * glm::cos(m_Pitch),
			glm::sin(m_Pitch),
			glm::cos(m_Yaw) * glm::cos(m_Pitch)));

		m_Pitch -= (float)M_PI / 2.0f;
		glm::vec3 up = -glm::normalize(glm::vec3(
			glm::sin(m_Yaw) * glm::cos(m_Pitch),
			glm::sin(m_Pitch),
			glm::cos(m_Yaw) * glm::cos(m_Pitch)));
		m_Pitch += (float)M_PI / 2.0f;
		glm::vec3 right = glm::cross(forward, up);

		//glm::vec3 right = GetRightVector();
		//glm::vec3 up = GetUpVector();
		m_FocusPoint -= right * delta.x * m_PanSpeed;
		m_FocusPoint += up * delta.y * m_PanSpeed;
	}

	void DebugCamera::UpdateTransform()
	{
		glm::vec3 focusToCamera = glm::normalize(glm::vec3(
			glm::sin(m_Yaw) * glm::cos(m_Pitch),
			glm::sin(m_Pitch),
			glm::cos(m_Yaw) * glm::cos(m_Pitch)));

		glm::vec3 position = m_FocusPoint + focusToCamera * m_Distance;
		glm::quat rotation(glm::vec3(m_Yaw, m_Pitch, 0));

		m_Transform.SetModel(glm::translate(position) * glm::toMat4(rotation));
		//m_Transform.SetPosition(position);
		//m_Transform.SetRotation(rotation);
	}

	void DebugCamera::UpdateViewMatrix()
	{
		/*m_Transform.SetPosition(-m_Transform.GetPosition());
		m_Transform.SetRotation(glm::conjugate(m_Transform.GetRotation()));
		m_ViewMatrix = m_Transform.GetLocalModel();
		m_Transform.SetPosition(-m_Transform.GetPosition());
		m_Transform.SetRotation(glm::conjugate(m_Transform.GetRotation()));*/
		//m_ViewMatrix = glm::inverse(m_Transform.GetLocalModel());
		glm::vec3 focusToCamera = glm::normalize(glm::vec3(
			glm::sin(m_Yaw) * glm::cos(m_Pitch),
			glm::sin(m_Pitch),
			glm::cos(m_Yaw) * glm::cos(m_Pitch)));

		glm::vec3 position = m_FocusPoint + focusToCamera * m_Distance;
		m_ViewMatrix = glm::lookAt(position, m_FocusPoint, glm::vec3(0, 1, 0));
	}

} }