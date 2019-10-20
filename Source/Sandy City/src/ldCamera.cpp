#include "ldCamera.h"

LDCamera::LDCamera(Camera* c)
{
	m_Camera = c;
}

LDCamera::~LDCamera()
{

}

void LDCamera::Init()
{

}

void LDCamera::CleanUp()
{

}

void LDCamera::Update()
{
	Input* i = Window::Instance->GetInput();
	glm::vec3 forward = glm::vec3(glm::sin(m_Angle), 0, glm::cos(m_Angle)) * m_MoveSpeed * Window::Instance->GetDeltaTime();
	glm::vec3 right = glm::vec3(glm::sin(m_Angle + M_PI / 2.0f), 0, glm::cos(m_Angle + M_PI / 2.0f)) * m_MoveSpeed * Window::Instance->GetDeltaTime();
	if (i->IsKeyDown(SDL_SCANCODE_W) || i->IsKeyDown(SDL_SCANCODE_UP))
	{
		m_Focus -= forward;
	}
	if (i->IsKeyDown(SDL_SCANCODE_S) || i->IsKeyDown(SDL_SCANCODE_DOWN))
	{
		m_Focus += forward;
	}
	if (i->IsKeyDown(SDL_SCANCODE_D) || i->IsKeyDown(SDL_SCANCODE_RIGHT))
	{
		m_Focus += right;
	}
	if (i->IsKeyDown(SDL_SCANCODE_A) || i->IsKeyDown(SDL_SCANCODE_LEFT))
	{
		m_Focus -= right;
	}

	if (m_Focus.x < m_MinAABB.x)
		m_Focus.x = m_MinAABB.x;
	if (m_Focus.z < m_MinAABB.y)
		m_Focus.z = m_MinAABB.y;

	if (m_Focus.x > m_MaxAABB.x)
		m_Focus.x = m_MaxAABB.x;
	if (m_Focus.z > m_MaxAABB.y)
		m_Focus.z = m_MaxAABB.y;

	if (i->IsMouseButtonDown(3))
	{
		m_Angle -= i->GetMouseDelta().x * m_RotateSpeed;
	}

	glm::vec3 focusToCamera = glm::normalize(glm::vec3(
		glm::sin(m_Angle) * glm::cos(m_DownAngle),
		glm::sin(m_DownAngle),
		glm::cos(m_Angle) * glm::cos(m_DownAngle)));

	float l = m_Distance * glm::cos(m_DownAngle);

	m_Camera->GetViewMatrix() = glm::lookAt(m_Focus + focusToCamera * l, m_Focus, glm::vec3(0, 1, 0));
}