#include "ducky_pch.h"
#include "terrainQuadTree.h"

namespace ducky { namespace graphics {

	TerrainQuadTree::TerrainQuadTree(const io::TerrainMeshData& td, uint x, uint y, uint lod, float dist, TerrainQuadTree* parent, TerrainQuadTree* root)
	{
		if (root == nullptr)
			m_Root = this;
		else m_Root = root;
		//m_Position = glm::vec3(x, y, td.modelData);
		m_LOD = lod;
		glm::vec3 tl = td.modelData.vertices[(x) * (int)pow(2, (m_LOD)) + (y) * (int)pow(2, (m_LOD)) * td.imageData.width].position;
		glm::vec3 tr = td.modelData.vertices[(x) * (int)pow(2, (m_LOD)) + (y + 1) * (int)pow(2, (m_LOD)) * td.imageData.width].position;
		glm::vec3 bl = td.modelData.vertices[(x + 1) * (int)pow(2, (m_LOD)) + (y) * (int)pow(2, (m_LOD)) * td.imageData.width].position;
		glm::vec3 br = td.modelData.vertices[(x + 1) * (int)pow(2, (m_LOD)) + (y + 1) * (int)pow(2, (m_LOD)) * td.imageData.width].position;
		m_Center = (tl + tr + bl + br) / 4.0f;
		m_Coords = glm::vec2(x, y);
		m_TerrainSize = glm::vec2(td.imageData.width, td.imageData.height);
		m_LOD = lod;
		m_Distance = dist;

		if (m_LOD != 0)
		{
			m_Children[0] = new TerrainQuadTree(td, x * 2 + 0, y * 2 + 0, lod - 1, dist, this, m_Root);
			m_Children[1] = new TerrainQuadTree(td, x * 2 + 1, y * 2 + 0, lod - 1, dist, this, m_Root);
			m_Children[2] = new TerrainQuadTree(td, x * 2 + 0, y * 2 + 1, lod - 1, dist, this, m_Root);
			m_Children[3] = new TerrainQuadTree(td, x * 2 + 1, y * 2 + 1, lod - 1, dist, this, m_Root);
		}

		m_Parent = parent;
		m_Status = 0;
		m_2AtLOD = powf(2, (int)m_LOD);
		m_DistMultiplier = m_2AtLOD;
	}

	TerrainQuadTree::~TerrainQuadTree()
	{
		delete m_Children[0];
		delete m_Children[1];
		delete m_Children[2];
		delete m_Children[3];
	}

	TerrainQuadTree* TerrainQuadTree::Trace(int x, int y, int lod, int twoAtLod)
	{
		if (x < 0 || y < 0 || x >= (int)m_TerrainSize.x / twoAtLod || y >= (int)m_TerrainSize.y / twoAtLod)
			return nullptr;
		int ix = x, iy = y;
		uint clod = m_Root->m_LOD;
		TerrainQuadTree* node = m_Root;
		int p = ((int)node->m_2AtLOD / twoAtLod);
		for(int i = 0; i < clod - lod; i++)
		{
			p /= 2;
			node = node->m_Children[x / p + (y / p) * 2];
			x %= p;
			y %= p;
		}
		if (node->m_Coords.x != ix || node->m_Coords.y != iy || lod != node->m_LOD)
			printf("sad");
		return node;
	}

	void TerrainQuadTree::ResetStatus()
	{
		if (m_Status != 0)
		{
			m_Children[0]->ResetStatus();
			m_Children[1]->ResetStatus();
			m_Children[2]->ResetStatus();
			m_Children[3]->ResetStatus();
		}
		m_Status = 0;
	}

	float GetDistanceH(const glm::vec3& a, const glm::vec3& b)
	{
		return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z);
	}

	void TerrainQuadTree::ConstructIBO(Camera* cam, const glm::vec3& campos, uint*& data, uint* base)
	{
		if (GetDistanceH(m_Center, campos) <= m_Distance * m_DistMultiplier && m_LOD > 0)
		{
			bool b = false;
			b |= m_Children[0]->ShouldRenderLOD(campos);
			b |= m_Children[1]->ShouldRenderLOD(campos);
			b |= m_Children[2]->ShouldRenderLOD(campos);
			b |= m_Children[3]->ShouldRenderLOD(campos);
			if (b)
			{
				m_Status = 2;
				m_Children[0]->AddToIBO(data, base);
				m_Children[1]->AddToIBO(data, base);
				m_Children[2]->AddToIBO(data, base);
				m_Children[3]->AddToIBO(data, base);
			}
			else
			{
				m_Status = 1;
				m_Children[0]->ConstructIBO(cam, campos, data, base);
				m_Children[1]->ConstructIBO(cam, campos, data, base);
				m_Children[2]->ConstructIBO(cam, campos, data, base);
				m_Children[3]->ConstructIBO(cam, campos, data, base);
			}
		}
	}

	bool TerrainQuadTree::ShouldRenderLOD(const glm::vec3& campos)
	{
		return !(GetDistanceH(m_Center, campos) <= m_Distance * m_DistMultiplier && m_LOD > 0);
	}

	void TerrainQuadTree::AddToIBO(uint*& data, uint* base)
	{
		m_Position = data - base;
		*data = (m_Coords.x) * m_2AtLOD + (m_Coords.y) * m_2AtLOD * m_TerrainSize.x;
		data++;
		*data = (m_Coords.x) * m_2AtLOD + (m_Coords.y + 1) * m_2AtLOD * m_TerrainSize.x;
		data++;
		*data = (m_Coords.x + 1) * m_2AtLOD + (m_Coords.y) * m_2AtLOD * m_TerrainSize.x;
		data++;
		*data = (m_Coords.x + 1) * m_2AtLOD + (m_Coords.y + 1) * m_2AtLOD * m_TerrainSize.x;
		data++;
		*data = 0xffffffff;
		data++;
	}

	inline void TerrainQuadTree::SetNull(uint* data)
	{
		data[m_Children[0]->m_Position] = 0;
		data[m_Children[0]->m_Position + 1] = 0;
		data[m_Children[0]->m_Position + 2] = 0;
		data[m_Children[0]->m_Position + 3] = 0;
		data[m_Children[1]->m_Position] = 0;
		data[m_Children[1]->m_Position + 1] = 0;
		data[m_Children[1]->m_Position + 2] = 0;
		data[m_Children[1]->m_Position + 3] = 0;
		data[m_Children[2]->m_Position] = 0;
		data[m_Children[2]->m_Position + 1] = 0;
		data[m_Children[2]->m_Position + 2] = 0;
		data[m_Children[2]->m_Position + 3] = 0;
		data[m_Children[3]->m_Position] = 0;
		data[m_Children[3]->m_Position + 1] = 0;
		data[m_Children[3]->m_Position + 2] = 0;
		data[m_Children[3]->m_Position + 3] = 0;
	}

	uint32 TerrainQuadTree::GetIBOIndex(int x, int y)
	{
		return (m_Children[0]->m_Coords.x + x) * m_Children[0]->m_2AtLOD + (m_Children[0]->m_Coords.y + y) * m_Children[0]->m_2AtLOD * m_TerrainSize.x;
	}

	void TerrainQuadTree::FillCracks(uint* data)
	{
		if (m_Status == 2)
		{
			TerrainQuadTree* right = Trace(m_Coords.x + 1, m_Coords.y, m_LOD, m_2AtLOD);
			TerrainQuadTree* left = Trace(m_Coords.x - 1, m_Coords.y, m_LOD, m_2AtLOD);
			TerrainQuadTree* bottom = Trace(m_Coords.x, m_Coords.y + 1, m_LOD, m_2AtLOD);
			TerrainQuadTree* top = Trace(m_Coords.x, m_Coords.y - 1, m_LOD, m_2AtLOD);
			byte leftHigher = left != nullptr ? (left->m_Status == 0 ? 1 : 0) : 0;
			byte rightHigher = right != nullptr ? (right->m_Status == 0 ? 1 : 0) : 0;
			byte bottomHigher = bottom != nullptr ? (bottom->m_Status == 0 ? 1 : 0) : 0;
			byte topHigher = top != nullptr ? (top->m_Status == 0 ? 1 : 0) : 0;
			uint32 key = (uint32)leftHigher | ((uint32)rightHigher << 1) | ((uint32)bottomHigher << 2) | ((uint32)topHigher << 3);
			switch (key)
			{
			case 0b0000:
				break;
			case 0b0001:
				data[m_Children[0]->m_Position + 0] = GetIBOIndex(1, 0);
				data[m_Children[0]->m_Position + 1] = GetIBOIndex(0, 0);
				data[m_Children[0]->m_Position + 2] = GetIBOIndex(1, 1);
				data[m_Children[0]->m_Position + 3] = GetIBOIndex(0, 2);
				data[m_Children[0]->m_Position + 4] = GetIBOIndex(1, 2);
				data[m_Children[0]->m_Position + 5] = 0xffffffff;
				data[m_Children[0]->m_Position + 6] = 0xffffffff;
				data[m_Children[0]->m_Position + 7] = 0xffffffff;
				data[m_Children[0]->m_Position + 8] = 0xffffffff;
				data[m_Children[0]->m_Position + 9] = 0xffffffff;

				data[m_Children[2]->m_Position + 0] = GetIBOIndex(1, 0);
				data[m_Children[2]->m_Position + 1] = GetIBOIndex(1, 1);
				data[m_Children[2]->m_Position + 2] = GetIBOIndex(2, 0);
				data[m_Children[2]->m_Position + 3] = GetIBOIndex(2, 1);
				data[m_Children[2]->m_Position + 4] = 0xffffffff;
				break;
			case 0b0010:
				data[m_Children[1]->m_Position + 0] = GetIBOIndex(1, 2);
				data[m_Children[1]->m_Position + 1] = GetIBOIndex(2, 2);
				data[m_Children[1]->m_Position + 2] = GetIBOIndex(1, 1);
				data[m_Children[1]->m_Position + 3] = GetIBOIndex(2, 0);
				data[m_Children[1]->m_Position + 4] = GetIBOIndex(1, 0);
				data[m_Children[1]->m_Position + 5] = 0xffffffff;
				data[m_Children[1]->m_Position + 6] = 0xffffffff;
				data[m_Children[1]->m_Position + 7] = 0xffffffff;
				data[m_Children[1]->m_Position + 8] = 0xffffffff;
				data[m_Children[1]->m_Position + 9] = 0xffffffff;

				data[m_Children[3]->m_Position + 0] = GetIBOIndex(0, 1);
				data[m_Children[3]->m_Position + 1] = GetIBOIndex(0, 2);
				data[m_Children[3]->m_Position + 2] = GetIBOIndex(1, 1);
				data[m_Children[3]->m_Position + 3] = GetIBOIndex(1, 2);
				data[m_Children[3]->m_Position + 4] = 0xffffffff;
				break;
			case 0b0011:
				data[m_Children[0]->m_Position + 0] = GetIBOIndex(1, 0);
				data[m_Children[0]->m_Position + 1] = GetIBOIndex(0, 0);
				data[m_Children[0]->m_Position + 2] = GetIBOIndex(1, 1);
				data[m_Children[0]->m_Position + 3] = GetIBOIndex(0, 2);
				data[m_Children[0]->m_Position + 4] = GetIBOIndex(1, 2);
				data[m_Children[0]->m_Position + 5] = 0xffffffff;
				data[m_Children[0]->m_Position + 6] = 0xffffffff;
				data[m_Children[0]->m_Position + 7] = 0xffffffff;
				data[m_Children[0]->m_Position + 8] = 0xffffffff;
				data[m_Children[0]->m_Position + 9] = 0xffffffff;

				data[m_Children[2]->m_Position + 0] = GetIBOIndex(1, 0);
				data[m_Children[2]->m_Position + 1] = GetIBOIndex(2, 0);
				data[m_Children[2]->m_Position + 2] = GetIBOIndex(1, 1);
				data[m_Children[2]->m_Position + 3] = GetIBOIndex(2, 2);
				data[m_Children[2]->m_Position + 4] = GetIBOIndex(1, 2);
				data[m_Children[2]->m_Position + 5] = 0xffffffff;
				data[m_Children[2]->m_Position + 6] = 0xffffffff;
				data[m_Children[2]->m_Position + 7] = 0xffffffff;
				data[m_Children[2]->m_Position + 8] = 0xffffffff;
				data[m_Children[2]->m_Position + 9] = 0xffffffff;
				break;
			case 0b0100:
				data[m_Children[2]->m_Position + 0] = GetIBOIndex(0, 1);
				data[m_Children[2]->m_Position + 1] = GetIBOIndex(0, 2);
				data[m_Children[2]->m_Position + 2] = GetIBOIndex(1, 1);
				data[m_Children[2]->m_Position + 3] = GetIBOIndex(2, 2);
				data[m_Children[2]->m_Position + 4] = GetIBOIndex(2, 1);
				data[m_Children[2]->m_Position + 5] = 0xffffffff;
				data[m_Children[2]->m_Position + 6] = 0xffffffff;
				data[m_Children[2]->m_Position + 7] = 0xffffffff;
				data[m_Children[2]->m_Position + 8] = 0xffffffff;
				data[m_Children[2]->m_Position + 9] = 0xffffffff;
				break;
			case 0b0101:
				data[m_Children[0]->m_Position + 0] = GetIBOIndex(0, 2);
				data[m_Children[0]->m_Position + 1] = GetIBOIndex(1, 1);
				data[m_Children[0]->m_Position + 2] = GetIBOIndex(0, 0);
				data[m_Children[0]->m_Position + 3] = GetIBOIndex(1, 0);
				data[m_Children[0]->m_Position + 4] = 0xffffffff;
				data[m_Children[2]->m_Position + 0] = GetIBOIndex(1, 1);
				data[m_Children[2]->m_Position + 1] = GetIBOIndex(0, 2);
				data[m_Children[2]->m_Position + 2] = GetIBOIndex(2, 1);
				data[m_Children[2]->m_Position + 3] = GetIBOIndex(2, 2);
				data[m_Children[2]->m_Position + 4] = 0xffffffff;
				data[m_Children[3]->m_Position + 0] = 0xffffffff;
				data[m_Children[3]->m_Position + 1] = 0xffffffff;
				data[m_Children[3]->m_Position + 2] = 0xffffffff;
				data[m_Children[3]->m_Position + 3] = 0xffffffff;
				data[m_Children[3]->m_Position + 4] = 0xffffffff;
				break;
			case 0b0110:
				data[m_Children[1]->m_Position + 0] = GetIBOIndex(1, 0);
				data[m_Children[1]->m_Position + 1] = GetIBOIndex(1, 1);
				data[m_Children[1]->m_Position + 2] = GetIBOIndex(2, 0);
				data[m_Children[1]->m_Position + 3] = GetIBOIndex(2, 2);
				data[m_Children[1]->m_Position + 4] = 0xffffffff;
				data[m_Children[2]->m_Position + 0] = GetIBOIndex(0, 1);
				data[m_Children[2]->m_Position + 1] = GetIBOIndex(0, 2);
				data[m_Children[2]->m_Position + 2] = GetIBOIndex(1, 1);
				data[m_Children[2]->m_Position + 3] = GetIBOIndex(2, 2);
				data[m_Children[2]->m_Position + 4] = 0xffffffff;
				data[m_Children[3]->m_Position + 0] = 0xffffffff;
				data[m_Children[3]->m_Position + 1] = 0xffffffff;
				data[m_Children[3]->m_Position + 2] = 0xffffffff;
				data[m_Children[3]->m_Position + 3] = 0xffffffff;
				data[m_Children[3]->m_Position + 4] = 0xffffffff;
				break;
			case 0b0111:
				SetNull(data);
				break;
			case 0b1000:
				data[m_Children[0]->m_Position + 0] = GetIBOIndex(2, 1);
				data[m_Children[0]->m_Position + 1] = GetIBOIndex(2, 0);
				data[m_Children[0]->m_Position + 2] = GetIBOIndex(1, 1);
				data[m_Children[0]->m_Position + 3] = GetIBOIndex(0, 0);
				data[m_Children[0]->m_Position + 4] = GetIBOIndex(0, 1);
				data[m_Children[0]->m_Position + 5] = 0xffffffff;
				data[m_Children[0]->m_Position + 6] = 0xffffffff;
				data[m_Children[0]->m_Position + 7] = 0xffffffff;
				data[m_Children[0]->m_Position + 8] = 0xffffffff;
				data[m_Children[0]->m_Position + 9] = 0xffffffff;
				break;
			case 0b1001:
				data[m_Children[0]->m_Position + 0] = GetIBOIndex(0, 0);
				data[m_Children[0]->m_Position + 1] = GetIBOIndex(1, 1);
				data[m_Children[0]->m_Position + 2] = GetIBOIndex(2, 0);
				data[m_Children[0]->m_Position + 3] = GetIBOIndex(2, 1);
				data[m_Children[0]->m_Position + 4] = 0xffffffff;
				data[m_Children[1]->m_Position + 0] = GetIBOIndex(0, 0);
				data[m_Children[1]->m_Position + 1] = GetIBOIndex(0, 2);
				data[m_Children[1]->m_Position + 2] = GetIBOIndex(1, 1);
				data[m_Children[1]->m_Position + 3] = GetIBOIndex(1, 2);
				data[m_Children[1]->m_Position + 4] = 0xffffffff;
				data[m_Children[2]->m_Position + 0] = 0xffffffff;
				data[m_Children[2]->m_Position + 1] = 0xffffffff;
				data[m_Children[2]->m_Position + 2] = 0xffffffff;
				data[m_Children[2]->m_Position + 3] = 0xffffffff;
				data[m_Children[2]->m_Position + 4] = 0xffffffff;
				break;
			case 0b1010:
				data[m_Children[0]->m_Position + 0] = GetIBOIndex(0, 1);
				data[m_Children[0]->m_Position + 1] = GetIBOIndex(1, 1);
				data[m_Children[0]->m_Position + 2] = GetIBOIndex(0, 0);
				data[m_Children[0]->m_Position + 3] = GetIBOIndex(2, 0);
				data[m_Children[0]->m_Position + 4] = 0xffffffff;
				data[m_Children[1]->m_Position + 0] = GetIBOIndex(2, 0);
				data[m_Children[1]->m_Position + 1] = GetIBOIndex(1, 1);
				data[m_Children[1]->m_Position + 2] = GetIBOIndex(2, 2);
				data[m_Children[1]->m_Position + 3] = GetIBOIndex(1, 2);
				data[m_Children[1]->m_Position + 4] = 0xffffffff;
				data[m_Children[3]->m_Position + 0] = 0xffffffff;
				data[m_Children[3]->m_Position + 1] = 0xffffffff;
				data[m_Children[3]->m_Position + 2] = 0xffffffff;
				data[m_Children[3]->m_Position + 3] = 0xffffffff;
				data[m_Children[3]->m_Position + 4] = 0xffffffff;
				break;
			case 0b1011:
				SetNull(data);
				break;
			case 0b1100:
				SetNull(data);
				break;
			case 0b1101:
				SetNull(data);
				break;
			case 0b1110:
				SetNull(data);
				break;
			case 0b1111:
				SetNull(data);
				break;
			default:
				break;
			}
		}
		else if(m_Status == 1)
		{
			m_Children[0]->FillCracks(data);
			m_Children[1]->FillCracks(data);
			m_Children[2]->FillCracks(data);
			m_Children[3]->FillCracks(data);
		}
	}

} }