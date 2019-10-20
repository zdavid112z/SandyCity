#include "ducky_pch.h"
#include "physicsScene.h"
#include "graphics\window.h"

namespace ducky { namespace core {

	PhysicsScene::PhysicsScene()
	{
		
	}

	PhysicsScene::~PhysicsScene()
	{
		for (int i = 0; i < m_RigidBodies.size(); i++)
			delete m_RigidBodies[i];
		for (int i = 0; i < m_Triggers.size(); i++)
			delete m_Triggers[i];
	}
	
	void PhysicsScene::AddRigidBody(RigidBody* b)
	{
		m_DynamicsWorld->addRigidBody(b->m_Body);
		m_RigidBodies.push_back(b);
	}

	void PhysicsScene::AddTrigger(PhysicsTrigger* b)
	{
		m_DynamicsWorld->addCollisionObject(b->m_GhostObject);
		m_Triggers.push_back(b);
	}

	void PhysicsScene::SetGravity(const glm::vec3& v)
	{
		m_DynamicsWorld->setGravity(btVector3(v.x, v.y, v.z));
	}

	void PhysicsScene::Update()
	{
		m_DynamicsWorld->stepSimulation(graphics::Window::Instance->GetDeltaTime());
	}

	RaycastResult PhysicsScene::Raycast(const glm::vec3& start, const glm::vec3& end)
	{
		RaycastResult result;
		btVector3 bstart(start.x, start.y, start.z);
		btVector3 bend(end.x, end.y, end.z);
		btCollisionWorld::ClosestRayResultCallback callback(bstart, bend);
		m_DynamicsWorld->rayTest(bstart, bend, callback);
		if (callback.hasHit())
		{
			const btVector3& pos = callback.m_hitPointWorld;
			result.position = glm::vec3(pos.getX(), pos.getY(), pos.getZ());
			const btVector3& normal = callback.m_hitNormalWorld;
			result.normal = glm::vec3(normal.getX(), normal.getY(), normal.getZ());
			result.fraction = callback.m_collisionObject->getFriction();
			result.object = (PhysicsObject*)(callback.m_collisionObject->getUserPointer());
		}
		return result;
	}

	vector<RaycastResult> PhysicsScene::RaycastAll(const glm::vec3& start, const glm::vec3& end)
	{
		vector<RaycastResult> result;
		RaycastResult ax;
		btVector3 bstart(start.x, start.y, start.z);
		btVector3 bend(end.x, end.y, end.z);
		btCollisionWorld::AllHitsRayResultCallback callback(bstart, bend);
		m_DynamicsWorld->rayTest(bstart, bend, callback);
		if (callback.hasHit())
		{
			for (uint i = 0; i < callback.m_collisionObjects.size(); i++)
			{
				const btVector3& pos = callback.m_hitPointWorld[i];
				ax.position = glm::vec3(pos.getX(), pos.getY(), pos.getZ());
				const btVector3& normal = callback.m_hitNormalWorld[i];
				ax.normal = glm::vec3(normal.getX(), normal.getY(), normal.getZ());
				ax.fraction = callback.m_hitFractions[i];
				ax.object = (PhysicsObject*)(callback.m_collisionObjects[i]->getUserPointer());
				result.push_back(ax);
			}
		}
		return result;
	}


	PhysicsScene3D::PhysicsScene3D()
	{
		m_CollisionConfiguration = new btDefaultCollisionConfiguration();
		m_CollisionDispathcher = new btCollisionDispatcher(m_CollisionConfiguration);
		m_BroadphaseInterface = new btDbvtBroadphase();
		m_Solver = new btSequentialImpulseConstraintSolver();
		m_DynamicsWorld = new btDiscreteDynamicsWorld(m_CollisionDispathcher, m_BroadphaseInterface, m_Solver, m_CollisionConfiguration);
		m_DynamicsWorld->setGravity(btVector3(0, -10, 0));
		m_DynamicsWorld->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	}

	PhysicsScene3D::~PhysicsScene3D()
	{
		delete m_DynamicsWorld;
		delete m_CollisionConfiguration;
		delete m_CollisionDispathcher;
		delete m_BroadphaseInterface;
		delete m_Solver;
	}


	PhysicsScene2D::PhysicsScene2D()
	{
		m_CollisionConfiguration = new btDefaultCollisionConfiguration();
		m_CollisionDispathcher = new btCollisionDispatcher(m_CollisionConfiguration);
		m_SimplexSolver = new btVoronoiSimplexSolver();
		m_PDSolver = new btMinkowskiPenetrationDepthSolver();
		m_ConvexAlgo2d = new btConvex2dConvex2dAlgorithm::CreateFunc(m_SimplexSolver, m_PDSolver);
		m_Box2dbox2dAlgo = new btBox2dBox2dCollisionAlgorithm::CreateFunc();
		
		m_CollisionDispathcher->registerCollisionCreateFunc(CONVEX_2D_SHAPE_PROXYTYPE, CONVEX_2D_SHAPE_PROXYTYPE, m_ConvexAlgo2d);
		m_CollisionDispathcher->registerCollisionCreateFunc(BOX_2D_SHAPE_PROXYTYPE, CONVEX_2D_SHAPE_PROXYTYPE, m_ConvexAlgo2d);
		m_CollisionDispathcher->registerCollisionCreateFunc(CONVEX_2D_SHAPE_PROXYTYPE, BOX_2D_SHAPE_PROXYTYPE, m_ConvexAlgo2d);
		m_CollisionDispathcher->registerCollisionCreateFunc(BOX_2D_SHAPE_PROXYTYPE, BOX_2D_SHAPE_PROXYTYPE, m_Box2dbox2dAlgo);

		m_BroadphaseInterface = new btDbvtBroadphase();
		m_Solver = new btSequentialImpulseConstraintSolver();

		m_DynamicsWorld = new btDiscreteDynamicsWorld(m_CollisionDispathcher, m_BroadphaseInterface, m_Solver, m_CollisionConfiguration);
		m_DynamicsWorld->setGravity(btVector3(0, -10, 0));
		m_DynamicsWorld->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	}

	PhysicsScene2D::~PhysicsScene2D()
	{
		delete m_DynamicsWorld;
		delete m_Solver;
		delete m_BroadphaseInterface;
		delete m_CollisionDispathcher;
		delete m_CollisionConfiguration;
		delete m_ConvexAlgo2d;
		delete m_PDSolver;
		delete m_SimplexSolver;
		delete m_Box2dbox2dAlgo;
	}

} }