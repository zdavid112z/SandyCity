#include "gui.h"
//#include "pyramid.h"

GUI::GUI(Shader* s)
{
	m_Shader = s;
	ImageData id;
	id.ReadFromFile("assets/textures/icons.png");
	m_Icons = new SpriteSheet(&id, new Texture2D(id, TextureParams()));
	m_Icons->GenerateAuto(2, 4);
	id.ReadFromFile("assets/textures/gui5.png");
	m_Overlay = new Texture2D(id, TextureParams());
	id.ReadFromFile("assets/textures/font5.png");
	m_FontAtlas = new Texture2D(id, TextureParams());
	id.Delete();
	m_Font.ReadFromFile("assets/fonts/font5.fnt");
	m_SelectSound = new AudioObject("assets/audio/select.wav");
	m_HoverSound = new AudioObject("assets/audio/hover.wav");
}

GUI::~GUI()
{
	delete m_HoverSound;
	delete m_SelectSound;
	delete m_Icons;
	delete m_Overlay;
	delete m_FontAtlas;
	m_Font.Delete();
}

void GUI::Init()
{
	m_Camera = m_Parent->GetComponent<LDCamera>();
	m_Money = m_Parent->GetComponent<MoneyManagement>();
	m_Builder = m_Parent->GetComponent<Builder>();
	m_Map = m_Parent->GetScene()->GetEntity("map")->GetComponent<GridMap>();

	ModelData md;
	md.ReadFromFile("assets/models/pyramid.dae", 0);
	ImageData id;
	id.ReadFromFile("assets/textures/pyramidThumb3.png");
	Texture2D* pyramidThumb = new Texture2D(id, TextureParams());
	id.ReadFromFile("assets/textures/pyramid3.png");

	m_Blueprints.push_back(new BuildingBlueprint(
		m_Map,
		glm::ivec2(1, 1),
		0, 0, 1000, 100,
		m_Shader,
		new Mesh(md),
		new LDMaterial("pyramid", new Texture2D(id, TextureParams()), LDUniformBuffer(1)),
		pyramidThumb,
		3, "Place them in a 3x3 to win!", "Pyramid"));

	md.ReadFromFile("assets/models/goldMine.dae", 0);
	id.ReadFromFile("assets/textures/goldThumb2.png");
	Texture2D* goldThumb = new Texture2D(id, TextureParams());
	id.ReadFromFile("assets/textures/goldMine4.png");

	m_Blueprints.push_back(new BuildingBlueprint(
		m_Map,
		glm::ivec2(1, 1),
		30, 0, 150, 0,
		m_Shader,
		new Mesh(md),
		new LDMaterial("goldMine", new Texture2D(id, TextureParams()), LDUniformBuffer(1)),
		goldThumb,
		1, "Place them on gold ore. Can have only two.\nWill be destroyed when there's no gold left.", "Gold Mine"));

	md.ReadFromFile("assets/models/temple.dae", 0);
	id.ReadFromFile("assets/textures/templeThumb.png");
	Texture2D* templeThumb= new Texture2D(id, TextureParams());
	id.ReadFromFile("assets/textures/temple.png");

	m_Blueprints.push_back(new BuildingBlueprint(
		m_Map,
		glm::ivec2(2, 2),
		0, 350, 1500, 0,
		m_Shader,
		new Mesh(md),
		new LDMaterial("temple", new Texture2D(id, TextureParams()), LDUniformBuffer(1)),
		templeThumb, 1, "", "Temple"));

	md.ReadFromFile("assets/models/shrine3.dae", 0);
	id.ReadFromFile("assets/textures/shrineThumb.png");
	Texture2D* shrineThumb = new Texture2D(id, TextureParams());
	id.ReadFromFile("assets/textures/shrine.png");

	m_Blueprints.push_back(new BuildingBlueprint(
		m_Map,
		glm::ivec2(1, 1),
		0, 40, 250, 0,
		m_Shader,
		new Mesh(md),
		new LDMaterial("shrine", new Texture2D(id, TextureParams()), LDUniformBuffer(1)),
		shrineThumb, 1, "", "Shrine"));

	glm::vec2 dim(1, 1);
	glm::vec2 center = dim / 2.0f;
	m_Money->AddBuilding(new Building(m_Blueprints[0], glm::ivec2(20, 20), Transform(glm::translate(glm::vec3(center.x, 0, center.y)) * glm::toMat4(glm::quat(glm::vec3(-M_PI / 2.0f, 0, 0))))));
}

void GUI::CleanUp()
{

}

void GUI::Render()
{

}

bool GUI::MouseOverGUI()
{
	glm::vec2 mouse = Window::Instance->GetInput()->GetMousePosition() * glm::vec2(32, 18) - glm::vec2(16, 9);
	mouse.y = -mouse.y;
	glm::vec2 c1 = glm::vec2(-13.97, 5.35);
	glm::vec2 d1 = glm::vec2(2.03, 3.65);
	glm::vec2 c2 = glm::vec2(-7.95, 8.25);
	glm::vec2 d2 = glm::vec2(30.05, 0.75);
	glm::vec2 delta1 = glm::abs(c1 - mouse);
	glm::vec2 delta2 = glm::abs(c2 - mouse);

	if (delta1.x <= d1.x && delta1.y <= d1.y)
		return true;
	if(delta2.x <= d2.x && delta2.y <= d2.y)
		return true;
	return false;
}

void GUI::RenderGUI()
{
	m_TotalGold = m_Money->m_Gold;
	m_GoldPerSecond = m_Money->m_GoldPerSecond;
	m_FreeEnergy = m_Money->m_TotalEnergy - m_Money->m_UsedEnergy;
	m_TotalEnergy = m_Money->m_TotalEnergy;
	Input* i = Window::Instance->GetInput();
	glm::vec2 mouse = i->GetMousePosition() * glm::vec2(32, 18) - glm::vec2(16, 9);
	mouse.y = -mouse.y;
	uint down = m_ButtonIndexNormal;
	uint up = m_ButtonIndexNormal;
	if (abs(mouse - m_DownButtonPosition).x < m_ButtonScale.x && abs(mouse - m_DownButtonPosition).y < m_ButtonScale.y)
		down = m_ButtonIndexHover;
	if (abs(mouse - m_UpButtonPosition).x < m_ButtonScale.x && abs(mouse - m_UpButtonPosition).y < m_ButtonScale.y)
		up = m_ButtonIndexHover;
	if (m_CurrentIndex == 0)
		up = m_ButtonIndexBlocked;
	if (m_CurrentIndex == m_Blueprints.size() - 1)
		down = m_ButtonIndexBlocked;

	if (up == m_ButtonIndexHover && i->GetMouseButtonState(1) == BUTTON_JUST_PRESSED && m_Render)
	{
		m_SelectSound->Play();
		m_CurrentIndex--;
		if (m_SelectedIndex != -1)
		{
			if (m_TotalGold >= m_Blueprints[m_CurrentIndex]->GetGoldCost() && m_FreeEnergy >= m_Blueprints[m_CurrentIndex]->GetEnergyCost())
				m_SelectedIndex = m_CurrentIndex;
			else m_SelectedIndex = -1;
		}
		m_Timer = glm::min(m_Timer, m_DurationForPopup - 0.3f);
	}

	if (down == m_ButtonIndexHover && i->GetMouseButtonState(1) == BUTTON_JUST_PRESSED && m_Render)
	{
		m_SelectSound->Play();
		m_CurrentIndex++;
		if (m_SelectedIndex != -1)
		{
			if (m_TotalGold >= m_Blueprints[m_CurrentIndex]->GetGoldCost() && m_FreeEnergy >= m_Blueprints[m_CurrentIndex]->GetEnergyCost())
				m_SelectedIndex = m_CurrentIndex;
			else m_SelectedIndex = -1;
		}
		m_Timer = glm::min(m_Timer, m_DurationForPopup - 0.3f);
	}

	if (up == m_ButtonIndexHover && m_LastHover != 1)
	{
		m_HoverSound->Play();
		m_LastHover = 1;
	}

	if (down == m_ButtonIndexHover && m_LastHover != 2)
	{
		m_HoverSound->Play();
		m_LastHover = 2;
	}

	if (up != m_ButtonIndexHover && down != m_ButtonIndexHover)
	{
		m_LastHover = 0;
	}

	if (i->GetMouseButtonState(2) == BUTTON_JUST_PRESSED && m_Render)
	{
		m_SelectedIndex = -1;
		m_SelectSound->Play();
	}

	uint thumbColor = m_Color;
	bool canbuy = true;
	if (m_TotalGold < m_Blueprints[m_CurrentIndex]->GetGoldCost() || m_FreeEnergy < m_Blueprints[m_CurrentIndex]->GetEnergyCost())
	{
		thumbColor = 0xffaa4455;
		canbuy = false;
	}

	bool thumb = glm::length2(mouse - m_ThumbnailPosition) < 1.5f * 1.5f;
	if (thumb && i->GetMouseButtonState(1) == BUTTON_JUST_PRESSED && canbuy && m_Render)
	{
		m_SelectedIndex = m_CurrentIndex;
		m_SelectSound->Play();
	}
	if (thumb)
		m_Timer += Window::Instance->GetDeltaTime();
	else m_Timer -= Window::Instance->GetDeltaTime();
	if (m_Timer < 0)
		m_Timer = 0;
	if (m_Timer > m_DurationForPopup * 2)
		m_Timer = m_DurationForPopup * 2;

	if (m_SelectedIndex != -1 && m_Render)
		m_Builder->SelectBlueprint(m_Blueprints[m_SelectedIndex]);
	else m_Builder->UnselectBlueprint();

	m_Parent->GetScene()->GetLayer<Layer2D>()->DrawImage (glm::mat4(), glm::vec4(16, 9, 0, 0), m_Overlay, 0, 1, 1, 0, 0, false, false, glm::vec2(0, 1), nullptr, m_Color, m_Color, m_Color, m_Color);
	m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_ButtonScale, m_DownButtonPosition), m_Icons->GetSprite(down), -1, false, false, glm::vec2(0, 1), nullptr, m_Color, m_Color, m_Color, m_Color);
	m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_ButtonScale, m_UpButtonPosition), m_Icons->GetSprite(up), -1, false, true, glm::vec2(0, 1), nullptr, m_Color, m_Color, m_Color, m_Color);
	m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_ThumbnailScale, m_ThumbnailPosition), m_Icons->GetSprite(5), -1, false, false, glm::vec2(0, 1), nullptr, m_Color, m_Color, m_Color, m_Color);
	m_Parent->GetScene()->GetLayer<Layer2D>()->DrawImage (glm::mat4(), glm::vec4(m_ThumbnailScale, m_ThumbnailPosition), m_Blueprints[m_CurrentIndex]->GetThumbnail(), -1, 1, 1, 0, 0, false, false, glm::vec2(0, 1), nullptr, thumbColor, thumbColor, thumbColor, thumbColor);

	m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_IconSize, m_TotalGoldPosition), m_Icons->GetSprite(2), -1, false, false, glm::vec2(0, 1), nullptr, m_Color, m_Color, m_Color, m_Color);
	m_Parent->GetScene()->GetLayer<Layer2D>()->DrawStringSimple(string("Total Gold: " + StringUtils::ToString(m_TotalGold, 10)).c_str(), &m_Font, m_FontAtlas, glm::vec3(m_TotalGoldPosition + glm::vec2(0.65f, -0.25f), 0), 0.011f, 0xffffffff, false, -1, glm::vec2(0, 1));

	m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_IconSize, m_GoldPerSecondPosition), m_Icons->GetSprite(7), -1, false, false, glm::vec2(0, 1), nullptr, m_Color, m_Color, m_Color, m_Color);
	m_Parent->GetScene()->GetLayer<Layer2D>()->DrawStringSimple(string("Gold Per Second: " + StringUtils::ToString(m_GoldPerSecond, 10)).c_str(), &m_Font, m_FontAtlas, glm::vec3(m_GoldPerSecondPosition + glm::vec2(0.65f, -0.25f), 0), 0.011f, 0xffffffff, false, -1, glm::vec2(0, 1));

	m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_IconSize, m_TotalEnergyPosition), m_Icons->GetSprite(4), -1, false, false, glm::vec2(0, 1), nullptr, m_Color, m_Color, m_Color, m_Color);
	m_Parent->GetScene()->GetLayer<Layer2D>()->DrawStringSimple(string("Total Energy: " + StringUtils::ToString(m_TotalEnergy, 10)).c_str(), &m_Font, m_FontAtlas, glm::vec3(m_TotalEnergyPosition + glm::vec2(0.65f, -0.25f), 0), 0.011f, 0xffffffff, false, -1, glm::vec2(0, 1));

	m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_IconSize, m_FreeEnergyPosition), m_Icons->GetSprite(3), -1, false, false, glm::vec2(0, 1), nullptr, m_Color, m_Color, m_Color, m_Color);
	m_Parent->GetScene()->GetLayer<Layer2D>()->DrawStringSimple(string("Free Energy: " + StringUtils::ToString(m_FreeEnergy, 10)).c_str(), &m_Font, m_FontAtlas, glm::vec3(m_FreeEnergyPosition + glm::vec2(0.65f, -0.25f), 0), 0.011f, 0xffffffff, false, -1, glm::vec2(0, 1));

	if (m_Timer >= m_DurationForPopup)
	{
		m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_InfoBoxSize, m_InfoBoxPosition), m_Icons->GetSprite(8), -2);
		m_Parent->GetScene()->GetLayer<Layer2D>()->DrawStringSimple(m_Blueprints[m_CurrentIndex]->GenInfo().c_str(), &m_Font, m_FontAtlas, glm::vec3(m_InfoTextPosition, 0), 0.011f, 0xffffffff, false, -3, glm::vec2(0.1f, 0.9f));
	}
	m_Render = false;
}

void GUI::Update()
{
	m_Render = true;
}