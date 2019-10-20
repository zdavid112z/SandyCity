#pragma once

#include "physicsShape.h"
#include "graphics\transform.h"
#include "physicsObject.h"
#include "../entity.h"
#include "../components/transformComponent.h"

namespace ducky { namespace core {
	
	class PhysicsScene;

	class RigidBody : public PhysicsObject
	{
	friend class PhysicsScene;
	public:
		RigidBody(Entity* entity, const glm::vec3& position, const glm::quat& rotation, PhysicsShape* shape, float mass) :
			PhysicsObject(entity)
		{
			m_Type = OBJECT_RIGID_BODY;
			m_Transform = nullptr;
			m_Shape = shape;
			m_Mass = mass;

			m_BTransform.setIdentity();
			m_BTransform.setOrigin(btVector3(position.x, position.y, position.z));
			m_BTransform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));

			Init();
		}

		RigidBody(Entity* entity, const glm::mat4& model, PhysicsShape* shape, float mass) :
			PhysicsObject(entity)
		{
			m_Type = OBJECT_RIGID_BODY;
			m_Transform = nullptr;
			m_Shape = shape;
			m_Mass = mass;

			m_BTransform.setFromOpenGLMatrix(&model[0][0]);

			Init();
		}

		RigidBody(Entity* entity, PhysicsShape* shape, float mass) :
			PhysicsObject(entity)
		{
			m_Type = OBJECT_RIGID_BODY;
			m_Transform = &(entity->GetComponent<TransformComponent>()->GetTransform());
			m_Shape = shape;
			m_Mass = mass;

			glm::mat4 m = m_Transform->GetLocalModel();
			m_BTransform.setFromOpenGLMatrix(&m[0][0]);

			Init();
		}

		void Init()
		{
			//m_BTransform.setIdentity();
			
			//m_BTransform.setOrigin(btVector3(transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z));
			//m_BTransform.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, transform->GetRotation().w));

			bool isDynamic = (m_Mass != 0);
			btVector3 localInertia(0, 0, 0);
			if (isDynamic)
				m_Shape->m_Shape->calculateLocalInertia(m_Mass, localInertia);

			btDefaultMotionState* myMotionState = new btDefaultMotionState(m_BTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(m_Mass, myMotionState, m_Shape->m_Shape, localInertia);
			m_Body = new btRigidBody(rbInfo);
			switch (m_Shape->GetType())
			{
			case SHAPE_BOX_2D:
			case SHAPE_SPHERE_2D:
				//m_Body->setActivationState(ISLAND_SLEEPING);
				m_Body->setLinearFactor(btVector3(1, 1, 0));
				m_Body->setAngularFactor(btVector3(0, 0, 1));
				break;
			}
			m_Body->setUserPointer(this);
		}

		virtual ~RigidBody()
		{

		}

		void Deactivate()
		{
			m_Body->forceActivationState(WANTS_DEACTIVATION);
		}

		void Activate()
		{
			m_Body->activate();
		}

		void SetFriction(float f)
		{
			m_Body->setFriction(f);
		}

		void SetHitFriction(float f)
		{
			m_Body->setHitFraction(f);
		}

		void SetRollingFriction(float f)
		{
			m_Body->setRollingFriction(f);
		}

		void SetLinearVelocity(const glm::vec3& v) 
		{
			m_Body->setLinearVelocity(btVector3(v.x, v.y, v.z));
		}

		void SetAngularVelocity(const glm::vec3& v)
		{
			m_Body->setAngularVelocity(btVector3(v.x, v.y, v.z));
		}

		void ClearForces()
		{
			m_Body->clearForces();
		}

		glm::vec3 GetPosition()
		{
			btVector3 v = m_Body->getWorldTransform().getOrigin();
			return glm::vec3(v.getX(), v.getY(), v.getZ());
		}

		glm::vec3 GetLinearVelocity() 
		{
			btVector3 v = m_Body->getLinearVelocity();
			return glm::vec3(v.getX(), v.getY(), v.getZ());
		}

		glm::vec3 GetAngularVelocity()
		{
			btVector3 v = m_Body->getAngularVelocity();
			return glm::vec3(v.getX(), v.getY(), v.getZ());
		}

		void ApplyForce(const glm::vec3& v)
		{	
			m_Body->applyCentralForce(btVector3(v.x, v.y, v.z));
		}

		void ApplyImpulse(const glm::vec3& v)
		{
			m_Body->applyCentralImpulse(btVector3(v.x, v.y, v.z));
		}

		void ApplyTorque(const glm::vec3& v)
		{
			m_Body->applyTorque(btVector3(v.x, v.y, v.z));
		}

		void ApplyTorqueImpulse(const glm::vec3& v)
		{
			m_Body->applyTorqueImpulse(btVector3(v.x, v.y, v.z));
		}

		void UpdateTransform()
		{
			if (m_Transform != nullptr)
			{
				m_Body->getMotionState()->getWorldTransform(m_BTransform);
				float v[16];
				m_BTransform.getOpenGLMatrix(v);
				m_Transform->SetModel(glm::mat4(
					v[0], v[1], v[2], v[3],
					v[4], v[5], v[6], v[7],
					v[8], v[9], v[10], v[11],
					v[12], v[13], v[14], v[15]));
			}
		}
	protected:
		btTransform m_BTransform;
		btRigidBody* m_Body;
		float m_Mass;
		PhysicsShape* m_Shape;
		graphics::Transform* m_Transform;
	};

} }