#pragma once

#include "../material.h"

namespace ducky { namespace graphics {

	struct TerrainMaterialBuffer
	{
		float heights[8];
		float numHeights = 0;
		float numPaths = 0;
		float tileX;
		float tileY;
		float scale;
		float normalStrength = 1;
		float padding1, padding2;
	};

	class TerrainMaterial : public Material
	{
	public:
		TerrainMaterial(const string& name) : Material(name)
		{
			BufferDesc bd;
			bd.data = nullptr;
			bd.dynamic = true;
			bd.size = sizeof(TerrainMaterialBuffer);
			bd.type = BUFFER_UNIFORM;
			m_UniformBuffer = new Buffer(bd);
		}
		virtual ~TerrainMaterial() {}

		TerrainMaterialBuffer& GetUniformData() { return m_UniformData; }
		vector<Texture2D*>& GetHeightTextures() { return m_HeightTextures; }
		vector<Texture2D*>& GetPaths() { return m_Paths; }

		void AddHeight(float mn, float mx, Texture2D* texture, Texture2D* normalmap)
		{
			m_UniformData.heights[(int)m_UniformData.numHeights * 2] = mn;
			m_UniformData.heights[(int)m_UniformData.numHeights * 2 + 1] = mx;
			m_UniformData.numHeights++;
			m_HeightTextures.push_back(texture);
			m_HeightTextures.push_back(normalmap);
		}

		void AddPath(Texture2D* tex, Texture2D* normalmap)
		{
			m_UniformData.numPaths++;
			m_Paths.push_back(tex);
			m_Paths.push_back(normalmap);
		}

		void Bind(Shader* s) override
		{
			m_UniformBuffer->SetData(&m_UniformData, 0, sizeof(TerrainMaterialBuffer));
			s->SetUniformBuffer("TerrainFragData", m_UniformBuffer);
			uint index = 0;
			for (uint i = 0; i < m_HeightTextures.size(); i++)
			{
				if(m_HeightTextures[i] != nullptr)
					m_HeightTextures[i]->Bind(index);
				else g_NormalDefault->Bind(index);
				index++;
			}
			for (uint i = 0; i < m_Paths.size(); i++)
			{
				if (m_Paths[i] != nullptr)
					m_Paths[i]->Bind(index);
				else g_NormalDefault->Bind(index);
				index++;
			}
		}

		void Unbind(Shader* s) override
		{

		}
	private:
		vector<Texture2D*> m_HeightTextures;
		vector<Texture2D*> m_Paths;
		TerrainMaterialBuffer m_UniformData;
		Buffer* m_UniformBuffer;

	};

} }