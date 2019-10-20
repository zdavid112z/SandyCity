#pragma once

#include "ducky.h"
#include "ldMaterial.h"
#include "building.h"

using namespace ducky;
using namespace graphics;
using namespace io;
using namespace core;

class GridMap;
class MoneyManagement;

class Builder : public Component
{
public:
	Builder(Shader* shader, Mesh* plane, Texture2D* white);
	~Builder();

	void Init() override;
	void CleanUp() override;

	void Update() override;
	void Render() override;

	string GetType() { return GetComponentType(); }
	static string GetComponentType() { return "Builder"; }

	void SelectBlueprint(BuildingBlueprint* b);
	void UnselectBlueprint();
private:
	bool m_Render;
	MoneyManagement* m_Money;
	bool m_UpdateMaterial;
	float m_Offset = 0.075f;
	float m_Height = 0.03f;
	GridMap* m_Map;
	BuildingBlueprint* m_Selected;
	Shader* m_Shader;
	Mesh* m_Plane;
	LDMaterial* m_Material;
	Transform m_Transform;
	AudioObject* m_BuildSound;
};