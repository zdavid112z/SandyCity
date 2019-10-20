#include "batteringram.h"
#include "gridMap.h"
#include <ctime>

BatteringRam::BatteringRam(Shader* s, GridMap* map)
{
	srand(time(NULL));
	m_Shader = s;
	m_HitSound = new AudioObject("assets/audio/select.wav");
	m_DestroySound = new AudioObject("assets/audio/destroy.wav");
	ModelData md;
	md.ReadFromFile("assets/models/batteringram.dae", 0);
	ImageData id;
	id.ReadFromFile("assets/textures/batteringram.png");
	Texture2D* tex = new Texture2D(id, TextureParams());
	m_Mesh = new Mesh(md);
	m_Material = new LDMaterial("battringRam", tex, LDUniformBuffer(1));
	m_Map = map;
	
	float angle = 0;
	glm::ivec2 pos;
	switch (m_Map->GetPathDirection())
	{
	case 0:
		m_Direction = glm::vec3(-1, 0, 0);
		angle = -M_PI / 2.0f;
		break;
	case 1:
		m_Direction = glm::vec3(0, 0, -1);
		angle = M_PI;
		break;
	case 2:
		m_Direction = glm::vec3(1, 0, 0);
		angle = M_PI / 2.0f;
		break;
	case 3:
		m_Direction = glm::vec3(0, 0, 1);
		angle = 0;
		break;
	};
	angle += M_PI / 2.0f;
	m_Rotation = glm::toMat4(glm::quat(glm::vec3(-M_PI / 2.0f, angle, 0)));
}

BatteringRam::~BatteringRam()
{
	delete m_Material;
	delete m_Mesh;
	delete m_HitSound;
}

void BatteringRam::Init()
{

}

void BatteringRam::CleanUp()
{

}

void BatteringRam::Update()
{
	m_NextSpawn -= Window::Instance->GetDeltaTime();
	if (m_NextSpawn <= 0)
	{
		m_NextSpawn = 2 + ((rand() % 10000) / 10000.0f) * 18;
		SpawnNew();
	}
	glm::vec2 m = Window::Instance->GetInput()->GetMousePosition() * 2.0f - 1.0f;
	m.y = -m.y;
	const float r = 0.175f;

	for (int i = 0; i < m_Instances.size(); i++)
	{
		m_Instances[i].position += m_Direction * m_Speed * Window::Instance->GetDeltaTime();

		glm::vec3 p3 = m_Instances[i].position + glm::vec3(0, 0.3f, 0);
		glm::vec4 f = m_Parent->GetScene()->GetCamera()->GetViewProjectionMatrix() * glm::vec4(m_Instances[i].position, 1.0f);
		glm::vec3 ff = glm::vec3(f) / f.w;
		
		if (glm::distance2(m, glm::vec2(ff)) < r * r && Window::Instance->GetInput()->GetMouseButtonState(1) == BUTTON_JUST_PRESSED)
		{
			m_Instances[i].clicks--;
			m_HitSound->Play();
			if (m_Instances[i].clicks <= 0)
			{
				m_Instances.erase(m_Instances.begin() + i);
				i--;
				m_DestroySound->Play();
				continue;
			}
		}

		if (glm::length2(m_Instances[i].position) <= 8)
		{
			m_Lost = true;
		}
	}
}

void BatteringRam::Render()
{
	for (uint i = 0; i < m_Instances.size(); i++)
	{
		DrawCall3D dc;
		dc.camera = m_Parent->GetScene()->GetCamera();
		dc.material = m_Material;
		dc.mesh = m_Mesh;
		dc.shader = m_Shader;
		dc.transform = glm::translate(m_Instances[i].position) * m_Rotation;
		dc.userdata = nullptr;
		m_Parent->GetScene()->GetLayer<Layer3D>()->AddDrawCall(dc);
	}
}

void BatteringRam::SpawnNew()
{
	BatteringRamInstance i;
	i.position = -m_Direction * float(m_Map->GetWidth() / 2) + glm::vec3(0.5f, 0, 0.5f);
	m_Instances.push_back(i);
}