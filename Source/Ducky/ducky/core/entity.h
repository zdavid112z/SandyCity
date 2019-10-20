#pragma once

#include "ducky_pch.h"

namespace ducky { namespace core {

	class Entity;
	class Scene;

	class Component
	{
		friend class Entity;
	public:
		Component() {}
		virtual ~Component() {}
		virtual void PreUpdate() {}
		virtual void Update() {}
		virtual void PostUpdate() {}

		virtual void PreRender() {}
		// true = cull
		virtual bool CullTest() { return false; }
		virtual void Render() {}
		virtual void PostRender() {}

		virtual void RenderGUI() {}
		virtual void Init() {}
		virtual void CleanUp() {}
		// GetComponentType is the static function
		virtual string GetType() = 0;
	protected:
		Entity* m_Parent;
	};

	class Entity
	{
		friend class Scene;
	public:
		Entity(Entity* parent, const string& name) : m_Parent(parent), m_Name(name) 
		{
			if (m_Parent != nullptr)
			{
				m_Parent->m_Children.push_back(this);
				m_Scene = m_Parent->m_Scene;
			}
		}
		virtual ~Entity()
		{
			for (uint i = 0; i < m_Children.size(); i++)
				delete m_Children[i];
			for (uint i = 0; i < m_Components.size(); i++)
			{
				m_Components[i]->CleanUp();
				delete m_Components[i];
			}
		}

		virtual void PreUpdate();
		virtual void Update();
		virtual void PostUpdate();

		virtual void PreRender();
		virtual void CullTest();
		virtual void Render();
		virtual void PostRender();

		virtual void RenderGUI();

		void AddComponent(Component* c) 
		{
			m_Components.push_back(c); 
			c->m_Parent = this;
			c->Init();
		}
		void RemoveComponent(Component* c)
		{
			for (uint i = 0; i < m_Components.size(); i++)
			{
				if (m_Components[i] == c)
				{
					m_Components.erase(m_Components.begin() + i);
					return;
				}
			}
		}

		void RemoveComponent(const string& type)
		{
			for (uint i = 0; i < m_Components.size(); i++)
			{
				if (m_Components[i]->GetType() == type)
				{
					m_Components.erase(m_Components.begin() + i);
					return;
				}
			}
		}

		template <class T>
		T* GetComponent()
		{
			for (uint i = 0; i < m_Components.size(); i++)
			{
				if (m_Components[i]->GetType() == T::GetComponentType())
				{
					return (T*)m_Components[i];
				}
			}
			return nullptr;
		}

		Entity* GetChildEntity(const string& name)
		{
			for (uint i = 0; i < m_Children.size(); i++)
			{
				if (m_Children[i]->GetName() == name)
					return m_Children[i];
				else
				{
					Entity* e = m_Children[i]->GetChildEntity(name);
					if (e != nullptr)
						return e;
				}
			}
			return nullptr;
		}

		void RemoveChildEntity(const string& name)
		{
			for (uint i = 0; i < m_Children.size(); i++)
			{
				if (m_Children[i]->GetName() == name)
				{
					delete m_Children[i];
					m_Children.erase(m_Children.begin() + i);
					break;
				}
			}
		}

		void RemoveChildEntity(Entity* e)
		{
			for (uint i = 0; i < m_Children.size(); i++)
			{
				if (m_Children[i] == e)
				{
					delete m_Children[i];
					m_Children.erase(m_Children.begin() + i);
					break;
				}
			}
		}

		Entity* GetParent() { return m_Parent; }
		Scene* GetScene() { return m_Scene; }
		const string& GetName() { return m_Name; }
	protected:
		Scene* m_Scene;
		Entity* m_Parent;
		vector<Component*> m_Components;
		vector<Entity*> m_Children;
		string m_Name;
		bool m_Culled = false;
	};

} }