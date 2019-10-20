#pragma once

#include "ducky.h"
#include "building.h"

using namespace ducky;
using namespace graphics;
using namespace core;
using namespace io;

class GridMap;

class MoneyManagement : public Component
{
public:
	MoneyManagement();
	~MoneyManagement();

	void Init() override;
	void CleanUp() override;

	void Update() override;
	void Render() override;
	void UpdateData();
	void AddBuilding(Building*);
	void RemoveBuilding(Building*);
	void RemoveAt(const glm::ivec2& v);
	bool HasWon() { return m_Won; }

	string GetType() { return GetComponentType(); }
	static string GetComponentType() { return "$$$"; }
public:
	bool m_Won = false;
	int m_Mines = 0;
	int m_MaxMines = 2;
	GridMap* m_Map;
	float m_Gold = 1000;
	float m_GoldPerSecond = 0;
	float m_TotalEnergy = 0;
	float m_UsedEnergy = 0;
	vector<Building*> m_Buildings;
	BuildingBlueprint* m_Pyramid2 = nullptr, *m_Pyramid3 = nullptr;
};