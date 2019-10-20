#pragma once

#include "ducky.h"
#include "moneyManagement.h"
#include "ldCamera.h"
#include "builder.h"
#include "gridMap.h"

using namespace ducky;
using namespace core;
using namespace graphics;

class GUI : public Component
{
public:
	GUI(Shader* s);
	~GUI();

	void Init() override;
	void CleanUp() override;

	void Update() override;
	void Render() override;
	void RenderGUI() override;
	bool MouseOverGUI();

	FontData* GetFontData() { return &m_Font; }
	Texture2D* GetFont() { return m_FontAtlas; }

	string GetType() { return GetComponentType(); }
	static string GetComponentType() { return "ldgui"; }
private:
	bool m_Render;
	Shader* m_Shader;
	Texture2D* m_FontAtlas;
	FontData m_Font;
	Texture2D* m_Overlay;
	SpriteSheet* m_Icons;
	LDCamera* m_Camera;
	MoneyManagement* m_Money;
	Builder* m_Builder;
	GridMap* m_Map;
	int m_CurrentIndex = 0;
	vector<BuildingBlueprint*> m_Blueprints;

	int m_SelectedIndex = -1;
	uint m_Color = 0xffffffff;
	uint m_ButtonIndexNormal = 0;
	uint m_ButtonIndexHover = 6;
	uint m_ButtonIndexBlocked = 1;
	glm::vec2 m_UpButtonPosition = glm::vec2(-14.25, 7);
	glm::vec2 m_DownButtonPosition = glm::vec2(-14.25, 3);
	glm::vec2 m_ButtonScale = glm::vec2(1.5f, 0.375f);
	glm::vec2 m_ThumbnailPosition = glm::vec2(-14.25, 5);
	glm::vec2 m_ThumbnailScale = glm::vec2(1.5f);

	glm::vec2 m_IconSize = glm::vec2(0.5f);
	glm::vec2 m_TotalGoldPosition = glm::vec2(-15.25f, 8.4f);
	glm::vec2 m_GoldPerSecondPosition = glm::vec2(-8.75f, 8.4f);
	glm::vec2 m_TotalEnergyPosition = glm::vec2(-1.1f, 8.4f);
	glm::vec2 m_FreeEnergyPosition = glm::vec2(6.0f, 8.4f);
	int m_GoldPerSecond = 0, m_TotalGold = 0, m_TotalEnergy = 0, m_FreeEnergy = 0;

	float m_DurationForPopup = 0.35f;
	float m_Timer = 0;
	glm::vec2 m_InfoBoxPosition = glm::vec2(-5.5f, 2.25f);
	glm::vec2 m_InfoBoxSize = glm::vec2(8, 2.5f);
	glm::vec2 m_InfoTextPosition = glm::vec2(-13.2, 4.0f);

	byte m_LastHover = 0;
	AudioObject* m_SelectSound;
	AudioObject* m_HoverSound;
};