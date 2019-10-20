#include "ducky_pch.h"
#include "entity.h"
#include "components\transformComponent.h"

namespace ducky { namespace core {

	void Entity::PreUpdate()
	{
		for (uint i = 0; i < m_Components.size(); i++)
		{
			m_Components[i]->PreUpdate();
		}

		for (uint i = 0; i < m_Children.size(); i++)
		{
			m_Children[i]->PreUpdate();
		}
	}

	void Entity::Update()
	{
		for (uint i = 0; i < m_Components.size(); i++)
		{
			m_Components[i]->Update();
		}
		TransformComponent* t = GetComponent<TransformComponent>();
		if (t != nullptr)
			graphics::TransformStack::PushBack(t->GetTransform().GetLocalModel());
		for (uint i = 0; i < m_Children.size(); i++)
		{
			m_Children[i]->Update();
		}
		if (t != nullptr)
			graphics::TransformStack::PopBack();
	}

	void Entity::PostUpdate()
	{
		for (uint i = 0; i < m_Components.size(); i++)
		{
			m_Components[i]->PostUpdate();
		}

		for (uint i = 0; i < m_Children.size(); i++)
		{
			m_Children[i]->PostUpdate();
		}
	}

	void Entity::CullTest()
	{
		m_Culled = false;
		for (uint i = 0; i < m_Components.size(); i++)
		{
			if (m_Components[i]->CullTest())
			{
				m_Culled = true;
				return;
			}
		}
		TransformComponent* t = GetComponent<TransformComponent>();
		if (t != nullptr)
			graphics::TransformStack::PushBack(t->GetTransform().GetLocalModel());
		for (uint i = 0; i < m_Children.size(); i++)
		{
			m_Children[i]->CullTest();
		}
		if (t != nullptr)
			graphics::TransformStack::PopBack();
	}

	void Entity::PreRender()
	{
		if (m_Culled)
			return;
		for (uint i = 0; i < m_Components.size(); i++)
		{
			m_Components[i]->PreRender();
		}
		for (uint i = 0; i < m_Children.size(); i++)
		{
			m_Children[i]->PreRender();
		}
	}

	void Entity::Render()
	{
		if (m_Culled)
			return;
		for (uint i = 0; i < m_Components.size(); i++)
		{
			m_Components[i]->Render();
		}
		TransformComponent* t = GetComponent<TransformComponent>();
		if (t != nullptr)
			graphics::TransformStack::PushBack(t->GetTransform().GetLocalModel());
		for (uint i = 0; i < m_Children.size(); i++)
		{
			m_Children[i]->Render();
		}
		if (t != nullptr)
			graphics::TransformStack::PopBack();
	}

	void Entity::PostRender()
	{
		if (m_Culled)
			return;
		for (uint i = 0; i < m_Components.size(); i++)
		{
			m_Components[i]->PostRender();
		}
		for (uint i = 0; i < m_Children.size(); i++)
		{
			m_Children[i]->PostRender();
		}
	}

	void Entity::RenderGUI()
	{
		for (uint i = 0; i < m_Components.size(); i++)
		{
			m_Components[i]->RenderGUI();
		}
		TransformComponent* t = GetComponent<TransformComponent>();
		if (t != nullptr)
			graphics::TransformStack::PushBack(t->GetTransform().GetLocalModel());
		for (uint i = 0; i < m_Children.size(); i++)
		{
			m_Children[i]->RenderGUI();
		}
		if (t != nullptr)
			graphics::TransformStack::PopBack();
	}

} }