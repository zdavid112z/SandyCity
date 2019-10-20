#include "gridMap.h"
#include "building.h"
#include "moneyManagement.h"
#include <ctime>

GridMap::GridMap(Shader* s, Mesh* plane, Texture2D* sand, int w, int h)
{
	m_Shader = s;
	m_Width = w;
	m_Height = h;

	m_Plane = plane;
	
	m_Map = new byte[m_Width * m_Height];
	for (int i = 0; i < m_Width * m_Height; i++)
		m_Map[i] = 0;

	m_Texture = sand;

	m_Material = new LDMaterial("map");
	m_Material->SetTexture(m_Texture);
	m_Material->UpdateBuffer();
}

GridMap::~GridMap()
{
	for (uint i = 0; i < m_RandomGen.size(); i++)
		delete m_RandomGen[i].blueprint;
	delete m_Material;
	delete m_Map;
	delete m_Path;
	delete m_PathEnd;
}

void GridMap::Init()
{
	m_Transform = m_Parent->GetComponent<TransformComponent>();
}

void GridMap::Generate()
{
	ModelData md;
	ImageData id;

	md.ReadFromFile("assets/models/rocks1.dae", 0);
	id.ReadFromFile("assets/textures/rock.png");

	m_RandomGen.push_back(RandomGen(
		new BuildingBlueprint(this,
			glm::ivec2(1, 1),
			0, 0, 0, 0,
			m_Shader,
			new Mesh(md),
			new LDMaterial("rocks", new Texture2D(id, TextureParams()), LDUniformBuffer(1)),
			nullptr),
		0.01f));

	md.ReadFromFile("assets/models/rocks2.dae", 0);

	m_RandomGen.push_back(RandomGen(
		new BuildingBlueprint(this,
			glm::ivec2(1, 1),
			0, 0, 0, 0,
			m_Shader,
			new Mesh(md),
			new LDMaterial("rocks", new Texture2D(id, TextureParams()), LDUniformBuffer(1)),
			nullptr),
		0.01f));

	md.ReadFromFile("assets/models/rocks3.dae", 0);

	m_RandomGen.push_back(RandomGen(
		new BuildingBlueprint(this,
			glm::ivec2(1, 1),
			0, 0, 0, 0,
			m_Shader,
			new Mesh(md),
			new LDMaterial("rocks", new Texture2D(id, TextureParams()), LDUniformBuffer(1)),
			nullptr),
		0.01f));

	md.ReadFromFile("assets/models/rocks21.dae", 0);

	m_RandomGen.push_back(RandomGen(
		new BuildingBlueprint(this,
			glm::ivec2(2, 2),
			0, 0, 0, 0,
			m_Shader,
			new Mesh(md),
			new LDMaterial("rocks", new Texture2D(id, TextureParams()), LDUniformBuffer(1)),
			nullptr),
		0.01f));

	md.ReadFromFile("assets/models/rocks22.dae", 0);

	m_RandomGen.push_back(RandomGen(
		new BuildingBlueprint(this,
			glm::ivec2(2, 2),
			0, 0, 0, 0,
			m_Shader,
			new Mesh(md),
			new LDMaterial("rocks", new Texture2D(id, TextureParams()), LDUniformBuffer(1)),
			nullptr),
		0.01f));

	md.ReadFromFile("assets/models/gold3.dae", 0);
	id.ReadFromFile("assets/textures/gold2.png");

	m_RandomGen.push_back(RandomGen(
		new BuildingBlueprint(this,
			glm::ivec2(1, 1),
			0, 0, 0, 0,
			m_Shader,
			new Mesh(md),
			new LDMaterial("gold", new Texture2D(id, TextureParams()), LDUniformBuffer(1)),
			nullptr,
			2),
		0.02f));

	md.ReadFromFile("assets/models/planeNormal.dae", 0);
	id.ReadFromFile("assets/textures/path.png");

	m_Path = new BuildingBlueprint(this,
		glm::ivec2(1, 1),
		0, 0, 0, 0,
		m_Shader,
		new Mesh(md),
		new LDMaterial("path", new Texture2D(id, TextureParams(true, true, false)), LDUniformBuffer(1)),
		nullptr);
	m_Path->GetMaterial()->GetFlags() = MATERIAL_TRANSPARENCY;

	id.ReadFromFile("assets/textures/pathend.png");

	m_PathEnd = new BuildingBlueprint(this,
		glm::ivec2(1, 1),
		0, 0, 0, 0,
		m_Shader,
		new Mesh(md),
		new LDMaterial("pathend", new Texture2D(id, TextureParams(true, true, false)), LDUniformBuffer(1)),
		nullptr);
	m_PathEnd->GetMaterial()->GetFlags() = MATERIAL_TRANSPARENCY;

	srand(time(NULL));
	MoneyManagement* m = m_Parent->GetScene()->GetEntity("player")->GetComponent<MoneyManagement>();

	int r = rand() % 4;
	m_PathDirection = r;
	for (int i = 2; i < m_Width / 2; i++)
	{
		BuildingBlueprint* ax = m_Path;
		if (i == 2)
			ax = m_PathEnd;
		float angle = 0;
		glm::ivec2 pos;
		switch (r)
		{
		case 0:
			pos = glm::ivec2(i, 0);
			angle = -M_PI / 2.0f;
			break;
		case 1:
			pos = glm::ivec2(0, i);
			angle = M_PI;
			break;
		case 2:
			pos = glm::ivec2(-i, 0);
			angle = M_PI / 2.0f;
			break;
		case 3:
			pos = glm::ivec2(0, -i);
			angle = 0;
			break;
		};
		glm::vec2 dim(1, 1);
		glm::vec2 center = glm::vec2(pos.x, pos.y) + dim / 2.0f;

		m->AddBuilding(new Building(ax, pos + glm::ivec2(m_Width / 2, m_Height / 2), Transform(glm::translate(glm::vec3(center.x, 0.01f, center.y)) * glm::toMat4(glm::quat(glm::vec3(-M_PI / 2.0f, angle, 0))) * glm::scale(glm::vec3(1.f)) )));
	}

	for (uint y = 0; y < m_Height; y++)
	{
		for (uint x = 0; x < m_Width; x++)
		{
			if (x >= 18 && x <= 22 && y >= 18 && y <= 22)
				continue;
			for (uint i = 0; i < m_RandomGen.size(); i++)
			{
				float r = (float)(rand() % 10000) / 10000.0f;
				if (r <= m_RandomGen[i].chance)
				{
					if (m_RandomGen[i].blueprint->CanBePlaced(glm::ivec2(x, y)))
					{
						float rot = float(rand() % 36) * M_PI / 18.0f;
						glm::vec2 dim = m_RandomGen[i].blueprint->GetDimensions();
						glm::vec2 center = glm::vec2((float)x - (float)m_Width / 2.0f, (float)y - (float)m_Height / 2.0f) + dim / 2.0f;
						
						m->AddBuilding(new Building(m_RandomGen[i].blueprint, glm::ivec2(x, y), Transform(glm::translate(glm::vec3(center.x, 0, center.y)) * glm::toMat4(glm::quat(glm::vec3(-M_PI / 2.0f, rot, 0))))));
						break;
					}
				}
			}
		}
	}
}

void GridMap::CleanUp()
{

}

void GridMap::Set(int x, int y, byte val)
{
	m_Map[y * m_Width + x] = val;
}

byte GridMap::Get(int x, int y)
{
	return m_Map[y * m_Width + x];
}

glm::vec3 GridMap::GetRaycastFromMouse()
{
	glm::vec2 mousePos = Window::Instance->GetInput()->GetMousePosition();
	mousePos = glm::vec2(mousePos.x * 2.0f - 1.0f, (1.0f - mousePos.y) * 2.0f - 1.0f);
	glm::vec4 ray = glm::inverse(m_Parent->GetScene()->GetCamera()->GetViewProjectionMatrix()) * glm::vec4(mousePos.x, mousePos.y, 1, 1);
	return glm::normalize(glm::vec3(ray) / ray.w);
}

glm::ivec2 GridMap::GetSelected()
{
	glm::vec3 ray = GetRaycastFromMouse();

	glm::vec3 c = m_Parent->GetScene()->GetCamera()->GetPosition();
	float l = -(c.y / ray.y);
	glm::vec3 f = c + ray * l;

	return glm::ivec2(f.x + m_Width / 2, f.z + m_Height / 2);
}

void GridMap::Render()
{
	DrawCall3D dc;
	dc.camera = m_Parent->GetScene()->GetCamera();
	dc.material = m_Material;
	dc.mesh = m_Plane;
	dc.shader = m_Shader;
	dc.transform = m_Transform->GetTransform().GetModel();
	dc.userdata = nullptr;
	m_Parent->GetScene()->GetLayer<Layer3D>()->AddDrawCall(dc);
}

void GridMap::Update()
{
	glm::ivec2 v = GetSelected();
}