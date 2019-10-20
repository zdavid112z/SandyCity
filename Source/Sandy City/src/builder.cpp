#include "builder.h"
#include "gridMap.h"
#include "moneyManagement.h"
#include "gui.h"

Builder::Builder(Shader* shader, Mesh* plane, Texture2D* white)
{
	m_Shader = shader;
	m_Plane = plane;
	m_Material = new LDMaterial("builder highlight"); // EBEBEB - white
	m_Material->SetTexture(white);// A1D490 - green
	m_Material->GetUniformData()->color = glm::vec4(0.8, 0.8, 0.8, 0.4);
	m_Material->UpdateBuffer();
	m_Material->GetFlags() = MATERIAL_TRANSPARENCY;
	m_UpdateMaterial = false;
	m_Selected = nullptr;
	m_BuildSound = new AudioObject("assets/audio/build.wav");
}

Builder::~Builder()
{
	delete m_BuildSound;
	delete m_Material;
}

void Builder::Init()
{
	m_Map = m_Parent->GetScene()->GetEntity("map")->GetComponent<GridMap>();
	m_Money = m_Parent->GetComponent<MoneyManagement>();
}
void Builder::CleanUp()
{

}

void Builder::Update()
{
	m_Render = true;
}

void Builder::Render() 
{
	if (!m_Render)
		return;
	glm::ivec2 selected = m_Map->GetSelected();
	if (selected.x < 0 || selected.y < 0 || selected.x >= m_Map->GetWidth() || selected.y >= m_Map->GetHeight())
		return;
	glm::ivec2 iselected = selected;
	selected -= glm::ivec2(m_Map->GetWidth() / 2, m_Map->GetHeight() / 2);
	glm::vec2 dim = m_Selected == nullptr ? glm::vec2(1, 1) : glm::vec2(m_Selected->GetDimensions());
	glm::vec2 center = glm::vec2(selected) + dim / 2.0f;
	dim = dim / 2.0f - glm::vec2(m_Offset, m_Offset);

	//if (m_UpdateMaterial)
	{
		if (iselected.x < 4 || iselected.y < 4 || iselected.x >= 36 || iselected.y >= 36)
		{
			m_Material->GetUniformData()->color = glm::vec4(0.2, 0.2, 0.2, 0.4);
		}
		else if (m_Selected == nullptr)
			m_Material->GetUniformData()->color = glm::vec4(0.8, 0.8, 0.8, 0.4);
		else if ((m_Selected->GetMaterial()->GetName() != "goldMine" && m_Selected->CanBePlaced(iselected)) || (m_Selected->GetMaterial()->GetName() == "goldMine" && m_Map->Get(iselected.x, iselected.y) == 2 && m_Money->m_Mines < m_Money->m_MaxMines))
		{
			bool inCenter = selected.x >= -1 && selected.x <= 1 && selected.y >= -1 && selected.y <= 1;
			if ((inCenter ^ m_Selected->GetMaterial()->GetName() == "pyramid") || (m_Money->m_Gold < m_Selected->GetGoldCost()) || ((m_Money->m_TotalEnergy - m_Money->m_UsedEnergy) < m_Selected->GetEnergyCost()))
			{
				m_Material->GetUniformData()->color = glm::vec4(0.9, 0.4, 0.5, 0.4);
			}
			else
			{
				m_Material->GetUniformData()->color = glm::vec4(0.5, 0.9, 0.4, 0.4);
				if (Window::Instance->GetInput()->GetMouseButtonState(1) == BUTTON_JUST_PRESSED)
				{
					if (!m_Parent->GetComponent<GUI>()->MouseOverGUI())
					{
						if (m_Selected->GetMaterial()->GetName() == "goldMine")
						{
							m_Money->RemoveAt(iselected);
						}
						m_BuildSound->Play();
						m_Money->AddBuilding(new Building(m_Selected, iselected, Transform(glm::translate(glm::vec3(center.x, 0, center.y)) * glm::toMat4(glm::quat(glm::vec3(-M_PI / 2.0f, (rand() % 4) * (M_PI / 2.0f), 0))))));
					}
				}
			}
		}
		else m_Material->GetUniformData()->color = glm::vec4(0.9, 0.4, 0.5, 0.4);
		m_Material->UpdateBuffer();
		m_UpdateMaterial = false;
	}

	DrawCall3D dc;
	dc.camera = m_Parent->GetScene()->GetCamera();
	dc.material = m_Material;
	dc.mesh = m_Plane;
	dc.shader = m_Shader;
	dc.transform = glm::translate(glm::vec3(center.x, m_Height, center.y)) * glm::toMat4(glm::quat(glm::vec3(-M_PI / 2.0f, 0, 0))) * glm::scale(glm::vec3(dim.x, dim.y, 1.0f));
	dc.userdata = nullptr;
	m_Parent->GetScene()->GetLayer<Layer3D>()->AddDrawCall(dc);
	m_Render = false;
}

void Builder::SelectBlueprint(BuildingBlueprint* b)
{
	m_Selected = b;
	m_UpdateMaterial = true;
}

void Builder::UnselectBlueprint()
{
	m_Selected = nullptr;
	m_UpdateMaterial = true;
}