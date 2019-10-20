#include "ducky_pch.h"
#include "game.h"
#ifndef DUCKY_DISABLE_OPENAL
#include "openal\ALFramework.h"
#endif
#include "utils\debug.h"

namespace ducky { namespace core {

	Game* Game::Instance = nullptr;

#ifdef DUCKY_WEB

	static void dispatch_main()
	{
		emscripten_set_main_loop([] () { Game::Instance->Loop(); }, 0, 1);
	}

#endif

	Game::Game(const graphics::WindowDesc& wd)
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		Instance = this;
		m_Window = new graphics::Window(wd);
#ifndef DUCKY_DISABLE_OPENAL
		ALFWInit();
		if (!ALFWInitOpenALDefaultDevice())
		{
			utils::Debug::Printer << FATAL << "Could not initialise OpenAL!" << ENDL;
			ALFWShutdown();
		}
#endif
	}

	Game::~Game()
	{
		delete m_Window;
		SDL_Quit();
#ifndef DUCKY_DISABLE_OPENAL
		ALFWShutdownOpenAL();
		ALFWShutdown();
#endif
	}

	void Game::Start()
	{
		m_Running = true;
#ifdef DUCKY_WEB
		dispatch_main();
#else
		while (ShouldRun())
			Loop();
#endif
	}

	void Game::SetScene(Scene* s)
	{
		m_NextScene = s;
	}

	void Game::PreUpdate()
	{
		m_Scene->PreUpdate();
	}

	void Game::UpdatePhysics()
	{
		m_Scene->UpdatePhysics();
	}

	void Game::Update()
	{
		m_Scene->Update();
	}

	void Game::PostUpdate()
	{
		m_Scene->PostUpdate();
	}

	void Game::PreRender()
	{
		m_Scene->PreRender();
	}

	void Game::CullTest()
	{
		m_Scene->CullTest();
	}

	void Game::Render()
	{
		m_Scene->Render();
	}

	void Game::PostRender()
	{
		m_Scene->PostRender();
	}
	
	void Game::RenderLayers()
	{
		m_Scene->RenderLayers();
	}

	void Game::RenderGUI()
	{
		m_Scene->RenderGUI();
	}

	void Game::RenderLayersGUI()
	{
		m_Scene->RenderLayersGUI();
	}

	void Game::Stop()
	{
		m_Running = false;
	}

	bool Game::ShouldRun()
	{
		return m_Running && !m_Window->GetInput()->ShouldClose();
	}

	void Game::Loop()
	{
		m_FrameTimer = m_Window->GetAbsoluteTime();

		PreUpdate();
		UpdatePhysics();
		Update();
		////////////////////////////////////////
		//                                    //
		// TODO: Update Animation System Here //
		//                                    //
		////////////////////////////////////////
		PostUpdate();

		CullTest();
		PreRender();
		Render();
		PostRender();
		RenderLayers();

		RenderGUI();
		RenderLayersGUI();

		m_FrameTime = (m_Window->GetAbsoluteTime() - m_FrameTimer) / 1000.0f;
		m_Window->Update();
		
		if (m_NextScene != nullptr)
		{
			delete m_Scene;
			m_Scene = m_NextScene;
			m_NextScene = nullptr;
		}
	}

} }