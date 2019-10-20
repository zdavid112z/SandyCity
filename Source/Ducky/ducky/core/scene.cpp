#include "ducky_pch.h"
#include "scene.h"
#include "graphics\layer\layer3d.h"
#include "graphics\layer\layer2d.h"

namespace ducky { namespace core {

	Scene::Scene()
	{
		m_PhysicsScene = new PhysicsScene2D();
		m_RootEntity = new Entity(nullptr, "Root");
		m_RootEntity->m_Scene = this;
		m_Layers.push_back(new graphics::Layer3D());
		m_Layers.push_back(new graphics::Layer2D());
		// TODO: Temp true, load from file
		m_LightSetup = new graphics::LightSetup(true);
	}

	Scene::~Scene()
	{
		CleanUp();
		delete m_RootEntity;
	}

	void Scene::Update()
	{
		m_RootEntity->Update();
	}

	void Scene::Render()
	{
		m_RootEntity->Render();
	}

	void Scene::PreUpdate()
	{
		m_RootEntity->PreUpdate();
	}

	void Scene::UpdatePhysics()
	{
		m_PhysicsScene->Update();
	}

	void Scene::PostUpdate()
	{
		m_RootEntity->PostUpdate();
	}

	void Scene::PreRender()
	{
		m_RootEntity->PreRender();
	}

	void Scene::CullTest()
	{
		m_RootEntity->CullTest();
	}

	void Scene::PostRender()
	{
		m_RootEntity->PostRender();
	}

	void Scene::RenderLayers()
	{
		m_LightSetup->UpdateBuffer();
		graphics::Layer::SortLayers(m_Layers);
		for (uint i = 0; i < m_Layers.size(); i++)
		{
			m_Layers[i]->Render(m_LightSetup);
		}
	}

	void Scene::RenderLayersGUI()
	{
		graphics::Layer::SortLayers(m_Layers);
		graphics::Camera* temp;
		for (uint i = 0; i < m_Layers.size(); i++)
		{
			temp = m_Layers[i]->GetCamera();
			m_Layers[i]->SetCamera(m_GUICamera);
			m_Layers[i]->Render(m_LightSetup);
			m_Layers[i]->SetCamera(temp);
		}
	}

	void Scene::RenderGUI()
	{
		m_RootEntity->RenderGUI();
	}

	graphics::Layer* Scene::GetLayer(const string& name)
	{
		for (graphics::Layer* e : m_Layers)
			if (e->GetName() == name)
				return e;
		return nullptr;
	}

	void Scene::AddEntity(Entity* e)
	{
		e->m_Scene = this;
		e->m_Parent = m_RootEntity;
		m_RootEntity->m_Children.push_back(e);
	}

} }