#pragma once

#include "ducky.h"

using namespace ducky;
using namespace core;
using namespace graphics;
using namespace io;

class MoneyManagement;
class GUI;
class BatteringRam;

class Menu : public Component
{
public:
	Menu(bool ingame);
	~Menu();

	void Init() override;
	void CleanUp() override;

	void Update() override;
	void RenderGUI() override;

	string GetType() { return GetComponentType(); }
	static string GetComponentType() { return "Menu"; }
	uint GetMenu() { return m_Menu; }
	bool IsVisible() { return m_Display; }
private:
	bool IsInside(const glm::vec2& pos, const glm::vec2& size, const glm::vec2& mouse);
private:
	byte m_LastHover = 0;
	bool m_Ingame, m_Display = true;
	uint m_Menu = 0;
	Texture2D* m_Background;
	Texture2D* m_Help;
	SpriteSheet* m_Buttons;
	glm::vec2 m_ButtonSize = glm::vec2(4, 1);
	glm::vec2 m_StartButton = glm::vec2(0, 3);
	glm::vec2 m_HelpButton = glm::vec2(0, 0);
	glm::vec2 m_QuitButton = glm::vec2(0, -3);

	float m_Timer;
	AudioObject* m_SelectAudio;
	AudioObject* m_HoverAudio;
	MoneyManagement* m_Money;
	BatteringRam* m_Ram;
	GUI* m_GUI;
};