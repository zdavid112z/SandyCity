#pragma once

#include "BulletCollision\CollisionDispatch\btGhostObject.h"
#include "physicsObject.h"

namespace ducky { namespace core {

	class Scene;
	class PhysicsScene;

	struct PhysicsCollisionPointData
	{
		PhysicsObject* collider;
		bool advanced;
		glm::vec3 positionA, positionB, normalB;
		// -1 -> is first body, 1 -> is second body
		float direction;
		float distance;
	};

	struct PhysicsTriggerObjectData
	{
		PhysicsObject* collider;
		bool checked;
	};

	class PhysicsTrigger : public PhysicsObject
	{
		friend PhysicsScene;
	public:
		PhysicsTrigger(Entity* entity, PhysicsShape* shape, void(*onActivate)(const PhysicsCollisionPointData&), void(*onEnter)(PhysicsObject*) = nullptr, void(*onExit)(PhysicsObject*) = nullptr);

		void Activate()
		{
			m_GhostObject->activate();
		}

		void Update();
	protected:
		PhysicsTrigger(Entity* entity) : PhysicsObject(entity) {}

	protected:
		PhysicsCollisionPointData m_Data;
		void(*m_Function)(const PhysicsCollisionPointData&);
		void(*m_OnEnter)(PhysicsObject*) = nullptr;
		void(*m_OnExit)(PhysicsObject*) = nullptr;
		btGhostObject* m_GhostObject;
		vector<PhysicsTriggerObjectData> m_Cache;
	};

	class PhysicsTriggerAdvanced : public PhysicsTrigger
	{
		friend PhysicsScene;
		public:
			PhysicsTriggerAdvanced(Entity* entity, PhysicsShape* shape, void(*onActivate)(const PhysicsCollisionPointData&));

			void Activate()
			{
				m_PairCachingGhostObject->activate();
			}

			void Update();
		protected:
			btPairCachingGhostObject* m_PairCachingGhostObject;
	};

} }
