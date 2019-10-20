#pragma once

#include "physicsShape.h"
#include "graphics\transform.h"

namespace ducky { namespace core {

	class PhysicsScene;
	class Entity;

	enum PhysicsObjectType
	{
		OBJECT_RIGID_BODY,
		OBJECT_TRIGGER,
		OBJECT_TRIGGER_ADVANCED,
	};

	class PhysicsObject
	{
	public:
		PhysicsScene* GetScene() { return m_Scene; }
		Entity* GetParent() { return m_Parent; }
		PhysicsObjectType GetType() { return m_Type; }
		virtual void Activate() {}
		virtual void SetFriction(float f) {}
		virtual void SetHitFriction(float f) {}
		virtual void SetRollingFriction(float f) {}
		virtual void SetLinearVelocity(const glm::vec3& v) {}
		virtual void SetAngularVelocity(const glm::vec3& v) {}
		virtual glm::vec3 GetLinearVelocity() { return glm::vec3(); }
		virtual glm::vec3 GetAngularVelocity() { return glm::vec3(); }
	protected:
		PhysicsObject() {}
		PhysicsObject(Entity* e) : m_Parent(e) {}
		Entity* m_Parent;
		PhysicsScene* m_Scene;
		PhysicsObjectType m_Type;
	};

} }