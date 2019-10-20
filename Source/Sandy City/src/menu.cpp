#include "menu.h"
#include "moneyManagement.h"
#include "gui.h"
#include "batteringram.h"

Menu::Menu(bool ingame)
{
	m_Menu = 0;
	m_Ingame = ingame;
	m_Display = !ingame;
	ImageData id;
	if (!ingame)
	{
		id.ReadFromFile("assets/textures/background1.png");
		m_Background = new Texture2D(id, TextureParams());
	}
	id.ReadFromFile("assets/textures/help.png");
	m_Help = new Texture2D(id, TextureParams());
	
	id.ReadFromFile("assets/textures/buttons.png");
	m_Buttons = new SpriteSheet(&id, new Texture2D(id, TextureParams()));
	m_Buttons->GenerateAuto(4, 4);
	id.Delete();

	m_SelectAudio = new AudioObject("assets/audio/select.wav");
	m_HoverAudio = new AudioObject("assets/audio/hover.wav");
}

Menu::~Menu()
{
	delete m_SelectAudio;
	delete m_HoverAudio;
	delete m_Buttons;
	if(m_Background)
		delete m_Background;
}

void Menu::Init()
{
	if (m_Ingame)
	{
		m_Money = m_Parent->GetScene()->GetEntity("player")->GetComponent<MoneyManagement>();
		m_GUI = m_Parent->GetScene()->GetEntity("player")->GetComponent<GUI>();
		m_Ram = m_Parent->GetScene()->GetEntity("map")->GetComponent<BatteringRam>();
	}
}

void Menu::CleanUp()
{

}

void Menu::Update()
{
	m_Timer += Window::Instance->GetDeltaTime();
}

bool Menu::IsInside(const glm::vec2& pos, const glm::vec2& size, const glm::vec2& mouse)
{
	glm::vec2 delta = glm::abs(pos - mouse);
	if(delta.x < size.x && delta.y < size.y)
		return true;
	return false;
}

void Menu::RenderGUI()
{
	if (m_Menu == 2)
	{
		m_Parent->GetScene()->GetLayer<Layer2D>()->DrawImage(glm::mat4(), glm::vec4(16, 9, 0, 0), m_Help, -3);
		if (Window::Instance->GetInput()->GetKeyState(SDL_SCANCODE_ESCAPE) == BUTTON_JUST_PRESSED ||
			Window::Instance->GetInput()->GetKeyState(SDL_SCANCODE_SPACE) == BUTTON_JUST_PRESSED ||
			Window::Instance->GetInput()->GetKeyState(SDL_SCANCODE_RETURN) == BUTTON_JUST_PRESSED)
		{
			m_Menu = 0;
			m_SelectAudio->Play();
		}
		return;
	}
	if (m_Ingame)
	{
		if (m_Money->HasWon())
		{
			m_Display = true;
			glm::vec2 m = Window::Instance->GetInput()->GetMousePosition() * glm::vec2(32, 18) - glm::vec2(16, 9);
			m.y = -m.y;
			uint c = 0x55000000;
			m_Parent->GetScene()->GetLayer<Layer2D>()->DrawRect(glm::mat4(), glm::vec4(16, 9, 0, 0), c, c, c, c, -3);
			m_Parent->GetScene()->GetLayer<Layer2D>()->DrawStringSimple(("You won in " + StringUtils::ToString(((int)m_Timer) / 60, 10) + " minutes and " + StringUtils::ToString(((int)m_Timer) & 60, 10) + " seconds!").c_str(), m_GUI->GetFontData(), m_GUI->GetFont(), glm::vec3(0, 2, 0), 0.011f, 0xffffffff, true, -4);

			bool q = IsInside(m_QuitButton, m_ButtonSize, m);
			if (q)
				m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_ButtonSize, m_QuitButton), m_Buttons->GetSprite(6), -4);
			else m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_ButtonSize, m_QuitButton), m_Buttons->GetSprite(4), -4);

			if (q && m_LastHover != 3)
			{
				m_LastHover = 3;
				m_HoverAudio->Play();
			}
			if (!q)
				m_LastHover = 0;
			
			if (Window::Instance->GetInput()->GetMouseButtonState(1) == BUTTON_JUST_PRESSED)
			{
				if (q) { m_SelectAudio->Play(); m_Menu = 3; }
			}
			return;
		}
		else if (m_Ram->HasLost())
		{
			m_Display = true;
			glm::vec2 m = Window::Instance->GetInput()->GetMousePosition() * glm::vec2(32, 18) - glm::vec2(16, 9);
			m.y = -m.y;
			uint c = 0x55000000;
			m_Parent->GetScene()->GetLayer<Layer2D>()->DrawRect(glm::mat4(), glm::vec4(16, 9, 0, 0), c, c, c, c, -3);
			m_Parent->GetScene()->GetLayer<Layer2D>()->DrawStringSimple(("You lost..\nYou played for " + StringUtils::ToString(((int)m_Timer) / 60, 10) + " minutes and " + StringUtils::ToString(((int)m_Timer) & 60, 10) + " seconds!").c_str(), m_GUI->GetFontData(), m_GUI->GetFont(), glm::vec3(0, 2, 0), 0.011f, 0xffffffff, true, -4);

			bool q = IsInside(m_QuitButton, m_ButtonSize, m);
			if (q)
				m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_ButtonSize, m_QuitButton), m_Buttons->GetSprite(6), -4);
			else m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_ButtonSize, m_QuitButton), m_Buttons->GetSprite(4), -4);

			if (q && m_LastHover != 3)
			{
				m_LastHover = 3;
				m_HoverAudio->Play();
			}
			if (!q)
				m_LastHover = 0;

			if (Window::Instance->GetInput()->GetMouseButtonState(1) == BUTTON_JUST_PRESSED)
			{
				if (q) { m_SelectAudio->Play(); m_Menu = 3; }
			}
			return;
		}
		else
		{
			if (Window::Instance->GetInput()->GetKeyState(SDL_SCANCODE_ESCAPE) == BUTTON_JUST_PRESSED)
			{
				m_SelectAudio->Play();
				m_Display = !m_Display;
			}
			if (!m_Display)
				return;

			glm::vec2 m = Window::Instance->GetInput()->GetMousePosition() * glm::vec2(32, 18) - glm::vec2(16, 9);
			m.y = -m.y;
			uint c = 0x55000000;
			m_Parent->GetScene()->GetLayer<Layer2D>()->DrawRect(glm::mat4(), glm::vec4(16, 9, 0, 0), c, c, c, c, -3);

			bool s = IsInside(m_StartButton, m_ButtonSize, m);
			if (s)
				m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_ButtonSize, m_StartButton), m_Buttons->GetSprite(7), -4);
			else m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_ButtonSize, m_StartButton), m_Buttons->GetSprite(5), -4);

			bool h = IsInside(m_HelpButton, m_ButtonSize, m);
			if (h)
				m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_ButtonSize, m_HelpButton), m_Buttons->GetSprite(3), -4);
			else m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_ButtonSize, m_HelpButton), m_Buttons->GetSprite(1), -4);

			bool q = IsInside(m_QuitButton, m_ButtonSize, m);
			if (q)
				m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_ButtonSize, m_QuitButton), m_Buttons->GetSprite(6), -4);
			else m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_ButtonSize, m_QuitButton), m_Buttons->GetSprite(4), -4);

			if (s && m_LastHover != 1)
			{
				m_LastHover = 1;
				m_HoverAudio->Play();
			}
			else if (h && m_LastHover != 2)
			{
				m_LastHover = 2;
				m_HoverAudio->Play();
			}
			else if (q && m_LastHover != 3)
			{
				m_LastHover = 3;
				m_HoverAudio->Play();
			}
			if (!s && !h && !q)
				m_LastHover = 0;

			if (Window::Instance->GetInput()->GetMouseButtonState(1) == BUTTON_JUST_PRESSED)
			{
				if (q) { m_Menu = 3; m_SelectAudio->Play(); }
				else if (h) { m_Menu = 2; m_SelectAudio->Play(); }
				else if (s) { m_Display = false; m_SelectAudio->Play(); }
			}
			return;
		}
	}
	glm::vec2 m = Window::Instance->GetInput()->GetMousePosition() * glm::vec2(32, 18) - glm::vec2(16, 9);
	m.y = -m.y;
	m_Parent->GetScene()->GetLayer<Layer2D>()->DrawImage(glm::mat4(), glm::vec4(16, 9, 0, 0), m_Background, -3);

	bool s = IsInside(m_StartButton, m_ButtonSize, m);
	if (s)
		m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_ButtonSize, m_StartButton), m_Buttons->GetSprite(2), -4);
	else m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_ButtonSize, m_StartButton), m_Buttons->GetSprite(0), -4);

	bool h = IsInside(m_HelpButton, m_ButtonSize, m);
	if (h)
		m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_ButtonSize, m_HelpButton), m_Buttons->GetSprite(3), -4);
	else m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_ButtonSize, m_HelpButton), m_Buttons->GetSprite(1), -4);

	bool q = IsInside(m_QuitButton, m_ButtonSize, m);
	if (q)
		m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_ButtonSize, m_QuitButton), m_Buttons->GetSprite(6), -4);
	else m_Parent->GetScene()->GetLayer<Layer2D>()->DrawSprite(glm::mat4(), glm::vec4(m_ButtonSize, m_QuitButton), m_Buttons->GetSprite(4), -4);

	if (s && m_LastHover != 1)
	{
		m_LastHover = 1;
		m_HoverAudio->Play();
	}
	else if (h && m_LastHover != 2)
	{
		m_LastHover = 2;
		m_HoverAudio->Play();
	}
	else if (q && m_LastHover != 3)
	{
		m_LastHover = 3;
		m_HoverAudio->Play();
	}
	if (!s && !h && !q)
		m_LastHover = 0;

	if (Window::Instance->GetInput()->GetMouseButtonState(1) == BUTTON_JUST_PRESSED)
	{
		if (q) { m_Menu = 3; m_SelectAudio->Play(); }
		else if (h) { m_Menu = 2; m_SelectAudio->Play(); }
		else if (s) { m_Menu = 1; m_SelectAudio->Play(); }
	}
}