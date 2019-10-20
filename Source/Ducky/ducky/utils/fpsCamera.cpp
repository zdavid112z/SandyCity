#include "ducky_pch.h"
#include "fpsCamera.h"
#include "graphics\window.h"

namespace ducky { namespace utils {

	FPSCamera::FPSCamera(const glm::mat4& projection)
	{
		m_ProjectionMatrix = projection;
	}

	void FPSCamera::Update()
	{
		graphics::Input* input = graphics::Window::Instance->GetInput();
		glm::vec2 delta = input->GetMouseDelta();

		m_Yaw -= delta.x * m_Sensitivity;
		m_Pitch -= delta.y * m_Sensitivity;
		if (m_Pitch < -M_PI / 2.0f + 0.001f)
			m_Pitch = -M_PI / 2.0f + 0.001f;
		if (m_Pitch > M_PI / 2.0f - 0.001f)
			m_Pitch = M_PI / 2.0f - 0.001f;

		float speed = m_Speed;
		if (input->IsKeyDown(SDL_SCANCODE_LSHIFT))
			speed = m_BoostSpeed;
		speed *= graphics::Window::Instance->GetDeltaTime();

		if (input->IsKeyDown(SDL_SCANCODE_W))
			m_Position += GetForwardVector() * speed;
		if (input->IsKeyDown(SDL_SCANCODE_S))
			m_Position -= GetForwardVector() * speed;
		if (input->IsKeyDown(SDL_SCANCODE_D))
			m_Position -= GetRightVector() * speed;
		if (input->IsKeyDown(SDL_SCANCODE_A))
			m_Position += GetRightVector() * speed;

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + GetForwardVector(), glm::vec3(0, 1, 0));
	}

	glm::vec3 FPSCamera::GetUpVector() const
	{
		glm::vec3 up = -glm::normalize(glm::vec3(
			glm::sin(m_Yaw) * glm::cos(m_Pitch - (float)M_PI / 2.0f),
			glm::sin(m_Pitch - (float)M_PI / 2.0f),
			glm::cos(m_Yaw) * glm::cos(m_Pitch - (float)M_PI / 2.0f)));
		return up;
	}

	glm::vec3 FPSCamera::GetRightVector() const
	{
		return glm::cross(GetUpVector(), GetForwardVector());
	}

	glm::vec3 FPSCamera::GetForwardVector() const
	{
		glm::vec3 forward = glm::normalize(glm::vec3(
			glm::sin(m_Yaw) * glm::cos(m_Pitch),
			glm::sin(m_Pitch),
			glm::cos(m_Yaw) * glm::cos(m_Pitch)));
		return forward;
	}

	glm::vec3 FPSCamera::GetPosition() const
	{
		return m_Position;
	}

} }