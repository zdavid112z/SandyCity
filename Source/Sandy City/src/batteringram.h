#pragma once

#include "ducky.h"
#include "ldMaterial.h"

using namespace ducky;
using namespace graphics;
using namespace io;
using namespace core;

class GridMap;

struct BatteringRamInstance
{
	int clicks = 20;
	glm::vec3 position;
};

class BatteringRam : public Component
{
public:
	BatteringRam(Shader* s, GridMap* map);
	~BatteringRam();

	void Init() override;
	void CleanUp() override;

	void Update() override;
	void Render() override;

	void SpawnNew();

	string GetType() { return GetComponentType(); }
	static string GetComponentType() { return "BatteringRam"; }

	bool HasLost() { return m_Lost; }
private:
	float m_NextSpawn = 5;
	bool m_Lost = false;
	float m_Speed = 0.75f;
	glm::vec3 m_Direction;
	glm::mat4 m_Rotation;
	GridMap* m_Map;
	Mesh* m_Mesh;
	LDMaterial* m_Material;
	Shader* m_Shader;
	vector<BatteringRamInstance> m_Instances;
	AudioObject* m_HitSound;
	AudioObject* m_DestroySound;
};