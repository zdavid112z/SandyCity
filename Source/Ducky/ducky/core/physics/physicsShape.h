#pragma once

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btBox2dShape.h"
#include "BulletCollision/CollisionShapes/btConvex2dShape.h"
#include "glm\glm.hpp"

namespace ducky { namespace core {

	class PhysicsTrigger;
	class PhysicsTriggerAdvanced;
	class PhysicsScene;
	class RigidBody;

	enum PhysicsShapeType
	{
		SHAPE_UNKNOWN, SHAPE_BOX, SHAPE_SPHERE, SHAPE_CAPSULE, SHAPE_CONVEX_HULL,
		SHAPE_BOX_2D, SHAPE_SPHERE_2D
	};

	class PhysicsShape
	{
	friend class PhysicsScene;
	friend class PhysicsTrigger;
	friend class PhysicsTriggerAdvanced;
	friend class RigidBody;
	public:
		virtual ~PhysicsShape()
		{
			if(m_Shape)
				delete m_Shape;
			if (m_ShapeChild)
				delete m_ShapeChild;
		}
		PhysicsShapeType GetType() { return m_Type; }
		void SetMargin(float f) { m_Shape->setMargin(f); }
	protected:
		PhysicsShape() { m_Type = SHAPE_UNKNOWN; }
	protected:
		btCollisionShape* m_Shape;
		btCollisionShape* m_ShapeChild = nullptr;
		PhysicsShapeType m_Type;
	};

	class BoxShape : public PhysicsShape
	{
	public:
		BoxShape(const glm::vec3& size)
		{
			m_Shape = new btBoxShape(btVector3(size.x, size.y, size.z));
			m_Size = size;
			m_Type = SHAPE_BOX;
		}

		glm::vec3 GetSize() { return m_Size; }
	protected:
		glm::vec3 m_Size;
	};

	class SphereShape : public PhysicsShape
	{
	public:
		SphereShape(float radius)
		{
			m_Shape = new btSphereShape(radius);
			m_Radius = radius;
			m_Type = SHAPE_SPHERE;
		}
		
		float GetRadius() { return m_Radius; }
	protected:
		float m_Radius;
	};

	class Box2DShape : public PhysicsShape
	{
	public:
		Box2DShape(const glm::vec2& size, float depth = 0.04f)
		{
			btVector3 points[4] = {
				btVector3(0, 0, 0),
				btVector3(size.x, 0, 0),
				btVector3(size.x, size.y, 0),
				btVector3(0, size.y, 0)
			};
			//m_ShapeChild = new btConvexHullShape(&points[0].getX(), 4);
			m_ShapeChild = new btBoxShape(btVector3(size.x, size.y, depth));
			m_Shape = new btConvex2dShape((btBoxShape*)m_ShapeChild);
			//m_Shape = new btBox2dShape(btVector3(size.x, size.y, depth));
			m_Size = size;
			m_Type = SHAPE_BOX_2D;
		}

		glm::vec2 GetSize() { return m_Size; }
	protected:
		glm::vec2 m_Size;
	};

	class Sphere2DShape : public PhysicsShape
	{
	public:
		Sphere2DShape(const glm::vec2& size, float depth = 0.04f)
		{
			m_ShapeChild = new btCylinderShapeZ(btVector3(size.x, size.y, depth));
			m_Shape = new btConvex2dShape((btCylinderShapeZ*)m_ShapeChild);
			
			m_Size = size;
			m_Type = SHAPE_SPHERE_2D;
		}

		glm::vec2 GetSize() { return m_Size; }
	protected:
		glm::vec2 m_Size;
	};

} }