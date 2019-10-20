#pragma once

#include "ducky.h"
#include "ldMaterial.h"

class GridMap;
class Building;

class BuildingBlueprint
{
	friend class Building;
public:
	BuildingBlueprint(GridMap* map, const glm::ivec2& dim, float gold, float energy, float goldCost, float energyCost, Shader* shader, Mesh* mesh, LDMaterial* material, Texture2D* thumbnail, byte id = 1, const string& notes = "", const string& name = "");
	~BuildingBlueprint();

	virtual bool CanBePlaced(const glm::ivec2& pos);

	GridMap* GetMap() { return m_Map; }
	const glm::ivec2& GetDimensions() { return m_Dimensions; }

	string GenInfo();
	Shader* GetShader() { return m_Shader; }
	float GetGold() { return m_Gold; }
	float GetEnergy() { return m_Energy; }
	float GetGoldCost() { return m_GoldCost; }
	float GetEnergyCost() { return m_EnergyCost; }
	Texture2D* GetThumbnail() { return m_Thumbnail; }
	Mesh* GetMesh() { return m_Mesh; }
	LDMaterial* GetMaterial() { return m_Material; }
	string GetNote() { return m_Note; }
	string GetName() { return m_Name; }
protected:
	string m_Name;
	string m_Note;
	byte m_ID;
	Shader* m_Shader;
	Mesh* m_Mesh;
	Texture2D* m_Thumbnail;
	LDMaterial* m_Material;
	glm::ivec2 m_Dimensions;
	GridMap* m_Map;
	float m_Gold, m_Energy;
	float m_GoldCost, m_EnergyCost;
};

class Building
{
public:
	Building(BuildingBlueprint* blueprint, const glm::ivec2& pos, const Transform& t);
	~Building() {}
	virtual void RemoveFromMap();
	virtual void Render();
	virtual void Update();

	float GetTimer() { return m_Timer; }
	const glm::ivec2& GetPosition() { return m_Position; }
	BuildingBlueprint* GetBlueprint() { return m_Blueprint; }
	Transform& GetTransform() { return m_Transform; }
	float& GetTimeLeft() { return m_TimeLeft; }
protected:
	float m_TimeLeft;
	float m_Timer = 0;
	Transform m_Transform;
	BuildingBlueprint* m_Blueprint;
	glm::ivec2 m_Position;
};