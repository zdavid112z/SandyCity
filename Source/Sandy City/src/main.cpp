#include "ducky.h"
#include "gridMap.h"
#include "ldCamera.h"
#include "builder.h"
#include "gui.h"
#include "menu.h"
#include "batteringram.h"

using namespace ducky;
using namespace graphics;
using namespace core;
using namespace utils;

class MenuScene : public Scene
{
public:
	MenuScene();
	void Init();
	void PreRender() override;
	void Update() override;
private:
	Entity* m_Menu;
};

class LDScene : public Scene
{
public:
	LDScene()
	{
		Init();
	}

	void Init()
	{
		ModelData md;
		md.ReadFromFile("assets/models/plane3.dae", 0);
		m_Plane = new Mesh(md);

		md.Delete();

		ImageData id = ImageUtils::CreateNew(64, 64, IFI_RGBA8, IF_RGBA, 0xffffffff);
		m_White = new Texture2D(id, TextureParams());
		id.ReadFromFile("assets/textures/map1.png");
		m_Sand = new Texture2D(id, TextureParams());

		Shader::AddLibrary("lighting.glsl", FileUtils::ReadTextFile("assets/shaders/lighting.glsl"));
		m_Camera = new Camera(glm::perspective(glm::radians(90.0f), (float)Window::Instance->GetDesc().width / (float)Window::Instance->GetDesc().height, 0.01f, 1000.0f));
		SetCamera(m_Camera);
		SetGUICamera(new Camera(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1000.0f, 1000.0f)));

		m_Shader = new Shader("assets/shaders/simple.vert", "assets/shaders/simple.frag");
		m_Shader->SetUniform1i("u_Texture", 0);

		m_Map = new Entity(nullptr, "map");
		AddEntity(m_Map);
		m_Map->AddComponent(new TransformComponent(Transform(glm::vec3(0, 0, 0), glm::quat(glm::vec3(-M_PI / 2.0f, 0, 0)), glm::vec3(40))));
		m_Map->AddComponent(new GridMap(m_Shader, m_Plane, m_Sand, 40, 40));

		m_Player = new Entity(nullptr, "player");
		AddEntity(m_Player);
		m_Player->AddComponent(new LDCamera(m_Camera));
		m_Player->AddComponent(new MoneyManagement());
		m_Player->AddComponent(new Builder(m_Shader, m_Plane, m_White));
		m_Player->AddComponent(new GUI(m_Shader));

		m_Map->GetComponent<GridMap>()->Generate();

		m_Map->AddComponent(new BatteringRam(m_Shader, m_Map->GetComponent<GridMap>()));

		m_Menu = new Entity(nullptr, "menu");
		AddEntity(m_Menu);
		m_Menu->AddComponent(new Menu(true));
	}

	void PreRender() override
	{
		//GLuint err = glGetError();
		//if (err != 0)
		//Debug::Printer << WARN << "OpenGL Error: " << err << ENDL;
		glClearColor(0, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Scene::PreRender();
	}

	void Update() override
	{
		m_FPS++;
		if(!m_Menu->GetComponent<Menu>()->IsVisible())
			Scene::Update();

		uint m = m_Menu->GetComponent<Menu>()->GetMenu();
		if(m == 3)
			Game::Instance->SetScene(new MenuScene());

		m_Timer += Window::Instance->GetDeltaTimeRaw();
		if (m_Timer >= 1)
		{
			m_Timer--;
			Debug::Printer << INFO << "FPS: " << m_FPS << ENDL;
			m_FPS = 0;
		}
	}
private:
	float m_Timer = 0;
	uint m_FPS = 0;

	Mesh* m_Plane;
	Texture2D* m_Sand;
	Texture2D* m_White;

	Entity* m_Menu;
	Shader* m_Shader;
	Entity* m_Player;
	Entity* m_Map;
	Camera* m_Camera;
};

MenuScene::MenuScene()
{
	Init();
}

void MenuScene::Init()
{
	SetGUICamera(new Camera(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1000.0f, 1000.0f)));
	m_Menu = new Entity(nullptr, "menu");
	AddEntity(m_Menu);
	m_Menu->AddComponent(new Menu(false));
}

void MenuScene::PreRender() 
{
	glClearColor(0, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Scene::PreRender();
}

void MenuScene::Update() 
{
	Scene::Update();

	uint m = m_Menu->GetComponent<Menu>()->GetMenu();
	if (m == 3)
		Window::Instance->Close();
	else if (m == 1)
	{
		Game::Instance->SetScene(new LDScene());
	}
}

class LDGame : public Game
{
public:
	LDGame(const WindowDesc& wd) :
		Game(wd)
	{
		m_Scene = new MenuScene();
		Init();
	}

	~LDGame()
	{
		CleanUp();
	}

	void Init()
	{
		m_Music = new AudioObject("assets/audio/music.wav");
		m_Music->SetGain(0.2f);
		m_Music->SetLooping(true);
		m_Music->Play();
	}

	void CleanUp()
	{
		m_Music->Stop();
		delete m_Music;
	}
private:
	AudioObject* m_Music;
};

int main(int argc, char** args)
{
	WindowDesc wd;
	wd.width = 1280;
	wd.height = 720;
	wd.title = "Sandy City";
	wd.openglMajorVersion = 3;
	wd.openglMinorVersion = 3;
	wd.mouseGrabbed = false;
	wd.msaa = 4;
	wd.vsync = 1;
	wd.fullscreen = 0;

	LDGame* game = new LDGame(wd);
	game->Start();
	delete game;
	return 0;
}