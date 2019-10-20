#pragma once

#include "../entity.h"
#include "../physics/physicsShape.h"
#include "../scene.h"
#include "transformComponent.h"
#include "utils\debug.h"

namespace ducky { namespace core {

	class RigidBodyComponent : public Component
	{
	public:
		RigidBodyComponent(PhysicsShape* shape, float mass)
		{
			m_Shape = shape;
			m_Mass = mass;
		}

		void Init() override
		{
			m_Body = new RigidBody(m_Parent, m_Shape, m_Mass);
			m_Parent->GetScene()->GetPhysicsScene()->AddRigidBody(m_Body);
		}

		string GetType() { return GetComponentType(); }
		static string GetComponentType() { return "RigidBody"; }
		RigidBody* GetRigidBody() { return m_Body; }

		void Update()
		{
			m_Body->UpdateTransform();
		}
	private:
		PhysicsShape* m_Shape;
		float m_Mass;
		RigidBody* m_Body;
	};

} }