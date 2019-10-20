#include "moneyManagement.h"
#include "gridMap.h"
#include <ctime>

MoneyManagement::MoneyManagement()
{

}

MoneyManagement::~MoneyManagement()
{

}

void MoneyManagement::Init()
{
	m_Map = m_Parent->GetScene()->GetEntity("map")->GetComponent<GridMap>();
}

void MoneyManagement::CleanUp()
{

}

void MoneyManagement::Render()
{
	for (int i = 0; i < m_Buildings.size(); i++)
	{
		m_Buildings[i]->Render();
	}
}

void MoneyManagement::Update()
{
	m_Gold += m_GoldPerSecond * Window::Instance->GetDeltaTime();
	for (int i = 0; i < m_Buildings.size(); i++)
	{
		if (m_Buildings[i]->GetBlueprint()->GetMaterial()->GetName() == "goldMine")
		{
			m_Buildings[i]->GetTimeLeft() -= Window::Instance->GetDeltaTime();
			if (m_Buildings[i]->GetTimeLeft() <= 0)
			{
				m_Mines--;
				m_Buildings[i]->RemoveFromMap();
				m_Buildings.erase(m_Buildings.begin() + i);
				UpdateData();
				break;
			}
		}
	}
}

void MoneyManagement::UpdateData()
{
	m_GoldPerSecond = 10;
	m_TotalEnergy = 100;
	m_UsedEnergy = 0;
	for (uint i = 0; i < m_Buildings.size(); i++)
	{
		m_TotalEnergy += m_Buildings[i]->GetBlueprint()->GetEnergy();
		m_UsedEnergy += m_Buildings[i]->GetBlueprint()->GetEnergyCost();
		m_GoldPerSecond += m_Buildings[i]->GetBlueprint()->GetGold();
	}
}

void MoneyManagement::AddBuilding(Building* b)
{
	m_Gold -= b->GetBlueprint()->GetGoldCost();
	m_Buildings.push_back(b);
	UpdateData(); 
	if (b->GetBlueprint()->GetMaterial()->GetName() == "goldMine")
	{
		m_Mines++;
		srand(time(NULL));
		b->GetTimeLeft() = 30 + ((float)(rand() % 100000) / 100000.0f) * 20;
	}
	else if (b->GetBlueprint()->GetMaterial()->GetName() == "pyramid" && b->GetBlueprint()->GetDimensions() == glm::ivec2(1, 1))
	{
		if (m_Pyramid2 == nullptr)
		{
			m_Pyramid2 = new BuildingBlueprint(
				b->GetBlueprint()->GetMap(),
				glm::ivec2(2, 2),
				0, 0, 0, 400,
				b->GetBlueprint()->GetShader(),
				b->GetBlueprint()->GetMesh(),
				b->GetBlueprint()->GetMaterial(),
				nullptr, 4);

			m_Pyramid3 = new BuildingBlueprint(
				b->GetBlueprint()->GetMap(),
				glm::ivec2(3, 3),
				0, 0, 0, 900,
				b->GetBlueprint()->GetShader(),
				b->GetBlueprint()->GetMesh(),
				b->GetBlueprint()->GetMaterial(),
				nullptr, 5);
		}
		if (m_Map->Get(b->GetPosition().x + 1, b->GetPosition().y) == 3 && m_Map->Get(b->GetPosition().x, b->GetPosition().y + 1) == 3 && m_Map->Get(b->GetPosition().x + 1, b->GetPosition().y + 1) == 3)
		{
			RemoveAt(glm::ivec2(b->GetPosition().x, b->GetPosition().y));
			RemoveAt(glm::ivec2(b->GetPosition().x + 1, b->GetPosition().y));
			RemoveAt(glm::ivec2(b->GetPosition().x, b->GetPosition().y + 1));
			RemoveAt(glm::ivec2(b->GetPosition().x + 1, b->GetPosition().y + 1));

			glm::vec2 dim = glm::vec2(2, 2);
			glm::vec2 center = glm::vec2(b->GetPosition() + glm::ivec2(0, 0)) - glm::vec2(m_Map->GetWidth() / 2.0f, m_Map->GetHeight() / 2.0f) + dim / 2.0f;

			AddBuilding(new Building(m_Pyramid2, glm::ivec2(b->GetPosition().x, b->GetPosition().y),
				Transform(glm::translate(glm::vec3(center.x, 0, center.y)) * glm::toMat4(glm::quat(glm::vec3(-M_PI / 2.0f, 0, 0))) * glm::scale(glm::vec3(2, 2, 2)))));
		}
		else if (m_Map->Get(b->GetPosition().x + 1, b->GetPosition().y) == 3 && m_Map->Get(b->GetPosition().x, b->GetPosition().y - 1) == 3 && m_Map->Get(b->GetPosition().x + 1, b->GetPosition().y - 1) == 3)
		{
			RemoveAt(glm::ivec2(b->GetPosition().x, b->GetPosition().y));
			RemoveAt(glm::ivec2(b->GetPosition().x + 1, b->GetPosition().y));
			RemoveAt(glm::ivec2(b->GetPosition().x, b->GetPosition().y - 1));
			RemoveAt(glm::ivec2(b->GetPosition().x + 1, b->GetPosition().y - 1));

			glm::vec2 dim = glm::vec2(2, 2);
			glm::vec2 center = glm::vec2(b->GetPosition() + glm::ivec2(0, -1)) - glm::vec2(m_Map->GetWidth() / 2.0f, m_Map->GetHeight() / 2.0f) + dim / 2.0f;

			AddBuilding(new Building(m_Pyramid2, glm::ivec2(b->GetPosition().x, b->GetPosition().y - 1),
				Transform(glm::translate(glm::vec3(center.x, 0, center.y)) * glm::toMat4(glm::quat(glm::vec3(-M_PI / 2.0f, 0, 0))) * glm::scale(glm::vec3(2, 2, 2)))));
		}
		else if (m_Map->Get(b->GetPosition().x - 1, b->GetPosition().y) == 3 && m_Map->Get(b->GetPosition().x, b->GetPosition().y + 1) == 3 && m_Map->Get(b->GetPosition().x - 1, b->GetPosition().y + 1) == 3)
		{
			RemoveAt(glm::ivec2(b->GetPosition().x, b->GetPosition().y));
			RemoveAt(glm::ivec2(b->GetPosition().x - 1, b->GetPosition().y));
			RemoveAt(glm::ivec2(b->GetPosition().x, b->GetPosition().y + 1));
			RemoveAt(glm::ivec2(b->GetPosition().x - 1, b->GetPosition().y + 1));

			glm::vec2 dim = glm::vec2(2, 2);
			glm::vec2 center = glm::vec2(b->GetPosition() + glm::ivec2(-1, 0)) - glm::vec2(m_Map->GetWidth() / 2.0f, m_Map->GetHeight() / 2.0f) + dim / 2.0f;

			AddBuilding(new Building(m_Pyramid2, glm::ivec2(b->GetPosition().x - 1, b->GetPosition().y),
				Transform(glm::translate(glm::vec3(center.x, 0, center.y)) * glm::toMat4(glm::quat(glm::vec3(-M_PI / 2.0f, 0, 0))) * glm::scale(glm::vec3(2, 2, 2)))));
		}
		else if (m_Map->Get(b->GetPosition().x - 1, b->GetPosition().y) == 3 && m_Map->Get(b->GetPosition().x, b->GetPosition().y - 1) == 3 && m_Map->Get(b->GetPosition().x - 1, b->GetPosition().y - 1) == 3)
		{
			RemoveAt(glm::ivec2(b->GetPosition().x, b->GetPosition().y));
			RemoveAt(glm::ivec2(b->GetPosition().x - 1, b->GetPosition().y));
			RemoveAt(glm::ivec2(b->GetPosition().x, b->GetPosition().y - 1));
			RemoveAt(glm::ivec2(b->GetPosition().x - 1, b->GetPosition().y - 1));

			glm::vec2 dim = glm::vec2(2, 2);
			glm::vec2 center = glm::vec2(b->GetPosition() + glm::ivec2(-1, -1)) - glm::vec2(m_Map->GetWidth() / 2.0f, m_Map->GetHeight() / 2.0f) + dim / 2.0f;

			AddBuilding(new Building(m_Pyramid2, glm::ivec2(b->GetPosition().x - 1, b->GetPosition().y - 1),
				Transform(glm::translate(glm::vec3(center.x, 0, center.y)) * glm::toMat4(glm::quat(glm::vec3(-M_PI / 2.0f, 0, 0))) * glm::scale(glm::vec3(2, 2, 2)))));
		}
	}

	if (b->GetBlueprint()->GetMaterial()->GetName() == "pyramid")
	{
		const glm::ivec2& p = b->GetPosition();
		for (int y = -2; y <= 2; y++)
		{
			for (int x = -2; x <= 2; x++)
			{
				bool ok = true;
				for (int yy = 0; yy < 3; yy++)
				{
					for (int xx = 0; xx < 3; xx++)
					{
						if (m_Map->Get(p.x + x + xx, p.y + y + yy) != 3 && m_Map->Get(p.x + x + xx, p.y + y + yy) != 4)
						{
							ok = false;
							break;
						}
					}
					if (!ok)
						break;
				}
				if (ok)
				{
					for (int yy = 0; yy < 3; yy++)
					{
						for (int xx = 0; xx < 3; xx++)
						{
							RemoveAt(glm::ivec2(p.x + x + xx, p.y + y + yy));
						}
					}
					glm::vec2 dim = glm::vec2(3, 3);
					glm::vec2 center = glm::vec2(p + glm::ivec2(x, y)) - glm::vec2(m_Map->GetWidth() / 2.0f, m_Map->GetHeight() / 2.0f) + dim / 2.0f;

					AddBuilding(new Building(m_Pyramid3, glm::ivec2(p.x + x, p.y + y),
						Transform(glm::translate(glm::vec3(center.x, 0, center.y)) * glm::toMat4(glm::quat(glm::vec3(-M_PI / 2.0f, 0, 0))) * glm::scale(glm::vec3(3, 3, 3)))));
					m_Won = true;
					return;
				}
			}
		}
	}
}

void MoneyManagement::RemoveBuilding(Building* b)
{
	for (uint i = 0; i < m_Buildings.size(); i++)
	{
		if (m_Buildings[i] == b)
		{
			m_Buildings[i]->RemoveFromMap();
			m_Buildings.erase(m_Buildings.begin() + i);
			UpdateData();
			break;
		}
	}
}

void MoneyManagement::RemoveAt(const glm::ivec2& v)
{
	for (uint i = 0; i < m_Buildings.size(); i++)
	{
		if (m_Buildings[i]->GetPosition() == v)
		{
			m_Buildings[i]->RemoveFromMap();
			m_Buildings.erase(m_Buildings.begin() + i);
			UpdateData();
			break;
		}
	}
}