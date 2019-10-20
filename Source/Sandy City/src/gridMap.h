#pragma once

#include "ducky.h"
#include "ldMaterial.h"

using namespace ducky;
using namespace core;
using namespace graphics;
using namespace io;

class MoneyManagement;
class BuildingBlueprint;
class BatteringRam;

struct RandomGen
{
	RandomGen(BuildingBlueprint* b, float c) :
		blueprint(b),
		chance(c) {}
	BuildingBlueprint* blueprint;
	float chance;
};

class GridMap : public Component
{
public:
	GridMap(Shader* shader, Mesh* plane, Texture2D* sand, int w, int h);
	~GridMap();
	void Init() override;
	void CleanUp() override;

	string GetType() { return GetComponentType(); }
	static string GetComponentType() { return "GridMap"; }

	void Set(int x, int y, byte val);
	byte Get(int x, int y);

	glm::ivec2 GetSelected();
	glm::vec3 GetRaycastFromMouse();
	void Generate();

	void Render() override;
	void Update() override;
	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }
	byte GetPathDirection() { return m_PathDirection; }
private:
	byte m_PathDirection;
	byte* m_Map;
	int m_Width, m_Height;
	Mesh* m_Plane;
	LDMaterial* m_Material;
	TransformComponent* m_Transform;
	Texture2D* m_Texture;
	Shader* m_Shader;
	vector<RandomGen> m_RandomGen;
	BuildingBlueprint* m_Path, *m_PathEnd;
};
