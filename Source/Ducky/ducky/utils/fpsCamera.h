#pragma once

#include "debugCamera.h"

namespace ducky { namespace utils {

	class FPSCamera : public graphics::Camera
	{
	public:
		FPSCamera(const glm::mat4& projection);
		void Update();
		glm::vec3 GetUpVector() const override;
		glm::vec3 GetRightVector() const override;
		glm::vec3 GetForwardVector() const override;
		glm::vec3 GetPosition() const override;
	private:
		glm::vec3 m_Position;
		float m_Yaw = 0, m_Pitch = 0;
		float m_Sensitivity = 2.5f;
		float m_Speed = 10;
		float m_BoostSpeed = 30;
	};

} }