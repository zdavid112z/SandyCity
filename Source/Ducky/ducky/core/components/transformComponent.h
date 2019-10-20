#pragma once

#include "../entity.h"
#include "graphics\transform.h"

namespace ducky { namespace core {

	class TransformComponent : public Component
	{
	public:
		TransformComponent(const graphics::Transform& tr = graphics::Transform()) { m_Transform = tr; }

		string GetType() { return GetComponentType(); }
		static string GetComponentType() { return "Transform"; }

		const graphics::Transform& GetTransform() const { return m_Transform; }
		graphics::Transform& GetTransform() { return m_Transform; }

	private:
		graphics::Transform m_Transform;
	};

} }