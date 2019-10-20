#pragma once

#include "graphics\window.h"
#include "entity.h"
#include "physics\physicsScene.h"
#include "graphics\layer\layer.h"
#include "graphics/camera.h"
#include "graphics\light.h"

namespace ducky { namespace core {

	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		virtual void Init() {}
		virtual void CleanUp() {}

		virtual void PreUpdate();
		virtual void UpdatePhysics();
		virtual void Update();
		virtual void PostUpdate();
		virtual void PreRender();
		virtual void CullTest();
		virtual void Render();
		virtual void PostRender();
		virtual void RenderLayers();
		virtual void RenderGUI();
		virtual void RenderLayersGUI();

		graphics::LightSetup* GetLightSetup() { return m_LightSetup; }
		Entity* GetEntity(const string& name) { return m_RootEntity->GetChildEntity(name); }
		graphics::Layer* GetLayer(const string& name);
		void AddEntity(Entity* e);
		void AddLayer(graphics::Layer* l) { m_Layers.push_back(l); }
		void RemoveEntity(Entity* e) { m_RootEntity->RemoveChildEntity(e); }
		void RemoveEntity(const string& name) { m_RootEntity->RemoveChildEntity(name); }

		template <class T>
		T* GetLayer()
		{
			for (uint i = 0; i < m_Layers.size(); i++)
			{
				if (T::GetLayerName() == m_Layers[i]->GetName())
				{
					return (T*)m_Layers[i];
				}
			}
			return nullptr;
		}

		graphics::Camera* GetCamera() { return m_Camera; }
		void SetCamera(graphics::Camera* c) { m_Camera = c; }
		graphics::Camera* GetGUICamera() { return m_GUICamera; }
		void SetGUICamera(graphics::Camera* c) { m_GUICamera = c; }
		PhysicsScene* GetPhysicsScene() { return m_PhysicsScene; }
	protected:
		graphics::Camera* m_Camera;
		graphics::Camera* m_GUICamera = nullptr;
		vector<graphics::Layer*> m_Layers;
		PhysicsScene* m_PhysicsScene;
		Entity* m_RootEntity;
		graphics::LightSetup* m_LightSetup;
	};

} }