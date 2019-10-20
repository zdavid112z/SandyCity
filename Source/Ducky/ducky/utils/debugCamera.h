#pragma once

#include "graphics\camera.h"
#include "graphics\transform.h"

namespace ducky { namespace utils {

	class DebugCamera : public graphics::Camera
	{
	public:
		DebugCamera(const glm::mat4& projection);
		void Update();
	protected:
		void Zoom(const glm::vec2& delta);
		void Rotate(const glm::vec2& delta);
		void Pan(const glm::vec2& delta);
		void UpdateViewMatrix();
		void UpdateTransform();
	protected:
		graphics::Transform m_Transform;
		glm::vec3 m_FocusPoint = glm::vec3(0);
		float m_Yaw = 0, m_Pitch = 0;
		float m_Distance = 10.0f;
		float m_ZoomSpeed = 10;
		float m_RotationSpeed = 10;
		float m_PanSpeed = 10;
	};

} }