#pragma once

#include <ducky_pch.h>
#include <glm\glm.hpp>
#include <glm\gtx/transform.hpp>
#include <glm\gtx/quaternion.hpp>

namespace ducky { namespace graphics {

	class TransformStack
	{
	public:
		static glm::mat4 GetBack() { return m_Stack.back(); }
		static void PushBack(const glm::mat4& mat) { m_Stack.push_back(mat * m_Stack.back()); }
		static void PopBack() { m_Stack.pop_back(); }
	protected:
		static vector<glm::mat4> m_Stack;
	};

	class Transform
	{
	public:
		Transform(const glm::vec3& pos = glm::vec3(0, 0, 0), const glm::quat& rot = glm::quat(glm::vec3(0, 0, 0)), const glm::vec3& scale = glm::vec3(1, 1, 1))
		{
			m_Model = glm::translate(pos) * glm::toMat4(rot) * glm::scale(scale);
		}

		Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale = glm::vec3(1, 1, 1))
		{
			m_Model = glm::translate(pos) * glm::toMat4(glm::quat(rot)) * glm::scale(scale);
		}

		Transform(const glm::mat4& mat)
		{
			m_Model = mat;
		}

		const glm::mat4& GetLocalModel()
		{
			return m_Model;
		}

		glm::mat4 GetModel()
		{
			return GetLocalModel() * TransformStack::GetBack();
		}

		void SetModel(const glm::mat4 m)
		{
			m_Model = m;
		}

		void Translate(const glm::vec3& v)
		{
			m_Model = glm::translate(m_Model, v);
		}

		void Rotate(float angle, const glm::vec3& axis)
		{
			m_Model = glm::rotate(m_Model, angle, axis);
		}

		void Scale(const glm::vec3& s)
		{
			m_Model = glm::scale(m_Model, s);
		}
	protected:
		glm::mat4 m_Model;
	/*public:
		Transform(const glm::vec3& pos = glm::vec3(0, 0, 0), const glm::quat& rot = glm::quat(glm::vec3(0, 0, 0)), const glm::vec3& scale = glm::vec3(1, 1, 1)) :
			m_Position(pos),
			m_Rotation(rot),
			m_Scale(scale) {}

		Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale = glm::vec3(1, 1, 1)) :
			m_Position(pos),
			m_Rotation(rot),
			m_Scale(scale) {}

		glm::mat4 GetLocalModel()
		{
			return glm::translate(m_Position) * glm::toMat4(m_Rotation) * glm::scale(m_Scale);
		}

		glm::mat4 GetModel()
		{
			return GetLocalModel() * TransformStack::GetBack();
		}

		void SetPosition(const glm::vec3& v) { m_Position = v; }
		void SetRotation(const glm::vec3& v) { m_Rotation = glm::quat(v); }
		void SetRotation(const glm::quat& q) { m_Rotation = q; }
		void SetScale(const glm::vec3& v) { m_Scale = v; }

		glm::vec3& GetPosition() { return m_Position; }
		glm::quat& GetRotation() { return m_Rotation; }
		glm::vec3& GetScale() { return m_Scale; }

		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::quat& GetRotation() const { return m_Rotation; }
		const glm::vec3& GetScale() const { return m_Scale; }

		glm::vec3 GetEuler() const { return glm::eulerAngles(m_Rotation); }
	protected:
		glm::quat m_Rotation;
		glm::vec3 m_Position;
		glm::vec3 m_Scale;*/
	};

} }