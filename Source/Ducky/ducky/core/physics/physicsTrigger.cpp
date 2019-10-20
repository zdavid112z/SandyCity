#include "ducky_pch.h"
#include "physicsTrigger.h"
#include "../scene.h"
#include "../entity.h"
#include "../components/transformComponent.h"

namespace ducky { namespace core {

	PhysicsTrigger::PhysicsTrigger(Entity* entity, PhysicsShape* shape, void(*onActivate)(const PhysicsCollisionPointData&), void(*onEnter)(PhysicsObject*), void(*onExit)(PhysicsObject*))
		: PhysicsObject(entity)
	{
		m_Type = OBJECT_TRIGGER;
		m_GhostObject = new btGhostObject();
		m_GhostObject->setCollisionShape(shape->m_Shape);
		btTransform trans;
		glm::mat4 model = entity->GetComponent<TransformComponent>()->GetTransform().GetModel();
		trans.setFromOpenGLMatrix(&model[0][0]);
		m_GhostObject->setWorldTransform(trans);
		m_Function = onActivate;
		m_OnEnter = onEnter;
		m_OnExit = onExit;
		m_GhostObject->setUserPointer(this);
		m_Data.advanced = false;
		m_GhostObject->setCollisionFlags(m_GhostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}

	void PhysicsTrigger::Update()
	{
		for (int i = 0; i < m_Cache.size(); i++)
		{
			m_Cache[i].checked = false;
		}
		for (int i = 0; i < m_GhostObject->getNumOverlappingObjects(); i++)
		{
			const btCollisionObject* object = m_GhostObject->getOverlappingObject(i);
			m_Data.collider = (PhysicsObject*)(object->getUserPointer());
			m_Function(m_Data);
			bool found = false;
			for (int i = 0; i < m_Cache.size(); i++)
			{
				if (m_Cache[i].collider == m_Data.collider)
				{
					m_Cache[i].checked = true;
					found = true;
				}
			}
			if (!found)
			{
				if(m_OnEnter != nullptr)
					m_OnEnter(m_Data.collider);
				PhysicsTriggerObjectData d;
				d.collider = m_Data.collider;
				d.checked = true;
				m_Cache.push_back(d);
			}
		}

		for (int i = 0; i < m_Cache.size(); i++)
		{
			if (m_Cache[i].checked == false)
			{
				if(m_OnExit != nullptr)
					m_OnExit(m_Cache[i].collider);
				m_Cache.erase(m_Cache.begin() + i);
				i--;
			}
		}
	}

	PhysicsTriggerAdvanced::PhysicsTriggerAdvanced(Entity* entity, PhysicsShape* shape, void(*onActivate)(const PhysicsCollisionPointData&))
		: PhysicsTrigger(entity)
	{
		m_Type = OBJECT_TRIGGER_ADVANCED;
		m_PairCachingGhostObject = new btPairCachingGhostObject();
		m_PairCachingGhostObject->setCollisionShape(shape->m_Shape);
		btTransform trans;
		glm::mat4 model = entity->GetComponent<TransformComponent>()->GetTransform().GetModel();
		trans.setFromOpenGLMatrix(&model[0][0]);
		m_PairCachingGhostObject->setWorldTransform(trans);
		m_Function = onActivate;
		m_PairCachingGhostObject->setUserPointer(this);
		m_Data.advanced = true;
		m_PairCachingGhostObject->setCollisionFlags(m_PairCachingGhostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		m_GhostObject = m_PairCachingGhostObject;
	}

	void PhysicsTriggerAdvanced::Update()
	{
		btManifoldArray manifoldArray;
		btBroadphasePairArray& pairArray = m_PairCachingGhostObject->getOverlappingPairCache()->getOverlappingPairArray();
		int numPairs = pairArray.size();
		for (uint i = 0; i < numPairs; i++)
		{
			manifoldArray.clear();
			const btBroadphasePair& pair = pairArray[i];
			btDiscreteDynamicsWorld* world = m_Parent->GetScene()->GetPhysicsScene()->m_DynamicsWorld;
			btBroadphasePair* collisionPair = world->getPairCache()->findPair(pair.m_pProxy0, pair.m_pProxy1);
			if (!collisionPair)
				continue;
			if (collisionPair->m_algorithm)
				collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);
			for (int j = 0; j < manifoldArray.size(); j++)
			{
				btPersistentManifold* manifold = manifoldArray[j];
				bool isFirstBody = manifold->getBody0() == m_PairCachingGhostObject;
				float direction = isFirstBody ? -1.0f : 1.0f;
				for (int p = 0; p < manifold->getNumContacts(); p++)
				{
					const btManifoldPoint& pt = manifold->getContactPoint(p);
					if (pt.getDistance() < 0.0f)
					{
						if (isFirstBody)
							m_Data.collider = (PhysicsObject*)(manifold->getBody1()->getUserPointer());
						else m_Data.collider = (PhysicsObject*)(manifold->getBody0()->getUserPointer());
						m_Data.direction = direction;
						btVector3 posA = pt.getPositionWorldOnA();
						m_Data.positionA = glm::vec3(posA.getX(), posA.getY(), posA.getZ());
						btVector3 posB = pt.getPositionWorldOnB();
						m_Data.positionB = glm::vec3(posB.getX(), posB.getY(), posB.getZ());
						btVector3 normalB = pt.m_normalWorldOnB;
						m_Data.normalB = glm::vec3(normalB.getX(), normalB.getY(), normalB.getZ());
						m_Data.distance = pt.getDistance();
						m_Function(m_Data);
					}
				}
			}
		}
	}

} }