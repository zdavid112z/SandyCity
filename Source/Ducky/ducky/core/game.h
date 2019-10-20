#pragma once

#include "scene.h"
#include "ducky_pch.h"
#include "entity.h"
#include "graphics\window.h"

namespace ducky { namespace core {

	class Game
	{
	public:
		static Game* Instance;
	public:
		Game(const graphics::WindowDesc& wd);
		virtual ~Game();
		virtual void Init() = 0;
		virtual void CleanUp() = 0;
		virtual void PreUpdate();
		virtual void UpdatePhysics();
		virtual void Update();
		virtual void PostUpdate();
		virtual void PreRender();
		virtual void CullTest();
		virtual void Render();
		virtual void RenderLayers();
		virtual void PostRender();
		virtual void RenderGUI();
		virtual void RenderLayersGUI();
		Scene* GetScene() { return m_Scene; }
		graphics::Window* GetWindow() { return m_Window; }
		virtual void Loop();
		virtual void Start();
		virtual void Stop();
		virtual bool ShouldRun();
		virtual void SetScene(Scene*);
		float GetFrameTime() { return m_FrameTime; }
	protected:
		uint32 m_FrameTimer;
		float m_FrameTime;
		bool m_Running = false;
		graphics::Window* m_Window;
		Scene* m_Scene, *m_NextScene = nullptr;
	};

} }