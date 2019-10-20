#pragma once

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

namespace ducky { namespace graphics {

	class Camera
	{
	public:
		Camera(const glm::mat4& projection = glm::mat4(1)) : m_ProjectionMatrix(projection) {}

		virtual glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }
		virtual glm::mat4& GetViewMatrix() { return m_ViewMatrix; }
		virtual const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		virtual const glm::mat4& GetViewMatrix() const  { return m_ViewMatrix; }
		virtual glm::mat4 GetViewProjectionMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }

		virtual glm::vec3 GetUpVector() const { return glm::vec3(m_ViewMatrix * glm::vec4(0, 1, 0, 0.0f)); }
		virtual glm::vec3 GetRightVector() const { return glm::vec3(m_ViewMatrix * glm::vec4(1, 0, 0, 0.0f)); }
		virtual glm::vec3 GetForwardVector() const { return glm::vec3(m_ViewMatrix * glm::vec4(0, 0, 1, 0.0f)); }
		virtual glm::vec3 GetPosition() const { glm::mat4 i = glm::inverse(m_ViewMatrix); return glm::vec3(i[3]) / i[3].w; }
	protected:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
	};

} }