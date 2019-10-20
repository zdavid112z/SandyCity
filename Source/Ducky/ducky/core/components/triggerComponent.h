#pragma once

#include "../entity.h"
#include "../physics/physicsTrigger.h"
#include "../physics/physicsScene.h"
#include "../scene.h"

namespace ducky { namespace core {

   	class TriggerComponent : public Component
	{
	public:
		TriggerComponent(PhysicsShape* shape, void(*onActivate)(const PhysicsCollisionPointData&), bool advanced = false, void(*onEnter)(PhysicsObject*) = nullptr, void(*onExit)(PhysicsObject*) = nullptr)
		{
			m_Shape = shape;
			m_TriggerFunction = onActivate;
			m_Advanced = advanced;
			m_OnEnter = onEnter;
			m_OnExit = onExit;
		}

		void Init() override
		{
			if (m_Advanced)
				m_Trigger = new PhysicsTriggerAdvanced(m_Parent, m_Shape, m_TriggerFunction);
			else m_Trigger = new PhysicsTrigger(m_Parent, m_Shape, m_TriggerFunction, m_OnEnter, m_OnExit);
			m_Parent->GetScene()->GetPhysicsScene()->AddTrigger(m_Trigger);
		}

		string GetType() { return GetComponentType(); }
		static string GetComponentType() { return "Trigger"; }
		PhysicsTrigger* GetTrigger() { return m_Trigger; }

		void Update()
		{
			m_Trigger->Update();
		}
	private:
		void(*m_TriggerFunction)(const PhysicsCollisionPointData&);
		void(*m_OnEnter)(PhysicsObject*);
		void(*m_OnExit)(PhysicsObject*);
		bool m_Advanced;
		PhysicsShape* m_Shape;
		PhysicsTrigger* m_Trigger;
	};

} }