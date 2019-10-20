#pragma once

#include "BulletCollision/CollisionShapes/btBox2dShape.h"
#include "BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.h"

#include "BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.h"
#include "BulletCollision\NarrowPhaseCollision\btVoronoiSimplexSolver.h"
#include "LinearMath/btAlignedObjectArray.h"
#include "rigidBody.h"
#include "physicsTrigger.h"
#include "glm\glm.hpp"

namespace ducky { namespace core {

	struct RaycastResult
	{
		PhysicsObject* object = nullptr;
		glm::vec3 position, normal;
		float fraction;
	};

	class PhysicsScene
	{
	friend class PhysicsTriggerAdvanced;
	public:
		PhysicsScene();
		virtual ~PhysicsScene();
		void SetGravity(const glm::vec3&);

		void AddRigidBody(RigidBody* b);
		void AddTrigger(PhysicsTrigger* b);

		void Update();
		RaycastResult Raycast(const glm::vec3& start, const glm::vec3& end);
		vector<RaycastResult> RaycastAll(const glm::vec3& start, const glm::vec3& end);
	protected:
		btDiscreteDynamicsWorld* m_DynamicsWorld;

		vector<RigidBody*> m_RigidBodies;
		vector<PhysicsTrigger*> m_Triggers;
	};

	class PhysicsScene2D : public PhysicsScene
	{
	public:
		PhysicsScene2D();
		virtual ~PhysicsScene2D();
	protected:
		btBroadphaseInterface* m_BroadphaseInterface;
		btCollisionDispatcher* m_CollisionDispathcher;
		btDefaultCollisionConfiguration* m_CollisionConfiguration;
		btConstraintSolver* m_Solver;
		btConvex2dConvex2dAlgorithm::CreateFunc* m_ConvexAlgo2d;
		btVoronoiSimplexSolver* m_SimplexSolver;
		btMinkowskiPenetrationDepthSolver* m_PDSolver;
		btBox2dBox2dCollisionAlgorithm::CreateFunc* m_Box2dbox2dAlgo;
	};

	class PhysicsScene3D : public PhysicsScene
	{
	public:
		PhysicsScene3D();
		virtual ~PhysicsScene3D();
	protected:
		btDefaultCollisionConfiguration* m_CollisionConfiguration;
		btCollisionDispatcher* m_CollisionDispathcher;
		btBroadphaseInterface* m_BroadphaseInterface;
		btSequentialImpulseConstraintSolver* m_Solver;
	};

} }