#include "building.h"
#include "gridMap.h"

BuildingBlueprint::BuildingBlueprint(GridMap* map, const glm::ivec2& dim, float gold, float energy, float goldCost, float energyCost, Shader* shader, Mesh* mesh, LDMaterial* material, Texture2D* thumbnail, byte id, const string& note, const string& name)
{
	m_Name = name;
	m_Note = note;
	m_ID = id;
	m_Map = map;
	m_Dimensions = dim;
	m_Gold = gold;
	m_Energy = energy;
	m_Mesh = mesh;
	m_Material = material;
	m_GoldCost = goldCost;
	m_EnergyCost = energyCost;
	m_Thumbnail = thumbnail;
	m_Shader = shader;
}

BuildingBlueprint::~BuildingBlueprint()
{
	delete m_Material->GetTexture();
	delete m_Mesh;
	delete m_Material;
	if(m_Thumbnail)
		delete m_Thumbnail;
}

string BuildingBlueprint::GenInfo()
{
	string res = m_Name + "\nThis building ";
	if (m_GoldCost != 0 && m_EnergyCost != 0)
	{
		res += " costs ";
		res += StringUtils::ToString(m_GoldCost, 10);
		res += " gold and uses ";
		res += StringUtils::ToString(m_EnergyCost, 10);
		res += " energy.\n";
	}
	else if (m_GoldCost != 0)
	{
		res += " costs ";
		res += StringUtils::ToString(m_GoldCost, 10);
		res += " gold.\n";
	}
	else if (m_EnergyCost != 0)
	{
		res += " uses ";
		res += StringUtils::ToString(m_EnergyCost, 10);
		res += " energy.\n";
	}
	if (m_Gold != 0 || m_Energy != 0)
	{
		res += "It produces ";
		if (m_Gold != 0)
		{
			res += StringUtils::ToString(m_Gold, 10);
			res += " gold.\n";
		}
		else
		{
			res += StringUtils::ToString(m_Energy, 10);
			res += " energy.\n";
		}
	}
	res += m_Note;
	return res;
}

bool BuildingBlueprint::CanBePlaced(const glm::ivec2& pos)
{
	bool ip = m_Material->GetName() == "pyramid";
	for (int y = 0; y < m_Dimensions.y; y++)
	{
		for (int x = 0; x < m_Dimensions.x; x++)
		{
			if (m_Map->Get(x + pos.x, y + pos.y) != 0 || ((pos.x + x >= 19 && pos.x + x <= 21 && pos.y + y >= 19 && pos.y + y <= 21) && !ip))
				return false;
		}
	}
	return true;
}

Building::Building(BuildingBlueprint* blueprint, const glm::ivec2& pos, const Transform& t)
{
	m_Blueprint = blueprint;
	m_Position = pos;
	m_Transform = t;

	for (int y = 0; y < m_Blueprint->m_Dimensions.y; y++)
	{
		for (int x = 0; x < m_Blueprint->m_Dimensions.x; x++)
		{
			m_Blueprint->m_Map->Set(x + m_Position.x, y + m_Position.y, m_Blueprint->m_ID);
		}
	}
}

void Building::RemoveFromMap()
{
	for (int y = 0; y < m_Blueprint->m_Dimensions.y; y++)
	{
		for (int x = 0; x < m_Blueprint->m_Dimensions.x; x++)
		{
			m_Blueprint->m_Map->Set(x + m_Position.x, y + m_Position.y, 0);
		}
	}
}

void Building::Render()
{
	DrawCall3D dc;
	dc.camera = Game::Instance->GetScene()->GetCamera();
	dc.material = m_Blueprint->GetMaterial();
	dc.mesh = m_Blueprint->GetMesh();
	dc.shader = m_Blueprint->GetShader();
	dc.transform = m_Transform.GetModel();
	dc.userdata = nullptr;
	Game::Instance->GetScene()->GetLayer<Layer3D>()->AddDrawCall(dc);
}

void Building::Update()
{
	m_Timer += Window::Instance->GetDeltaTime();
}