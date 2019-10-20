#include "ducky_pch.h"
#include "terrainLayer.h"
#include "utils\debug.h"

namespace ducky { namespace graphics {

	TerrainLayer::TerrainLayer(const io::TerrainMeshData& t) : Layer(GetLayerName(), 1)
	{
		m_TerrainData = t;
		m_Heightmap = new Texture2D(t.imageData, TextureParams());

		BufferDesc bd;
		bd.data = nullptr;
		bd.dynamic = true;
		bd.size = t.iboSize;
		bd.type = BUFFER_INDEX;
		m_IBO = new Buffer(bd);

		m_TerrainMesh = new Mesh(t.modelData, m_IBO, (t.imageData.width - 1) * (t.imageData.height - 1));
		//m_TerrainMesh = new Mesh(t.modelData);
		m_TerrainShader = new Shader("assets/shaders/terrain.vert", "assets/shaders/terrain.frag");
		m_QuadTree = new TerrainQuadTree(t, 0, 0, log2f(t.imageData.width - 1), t.distance, nullptr, nullptr);
		delete[] t.modelData.vertices;

		int tex[32];
		for (uint i = 0; i < 32; i++)
			tex[i] = i;
		m_TerrainShader->SetUniform1iv("u_Textures", tex, 32);
		
		bd.data = nullptr;
		bd.dynamic = true;
		bd.size = sizeof(TerrainUniformBuffer);
		bd.type = BUFFER_UNIFORM;
		m_UniformBuffer = new Buffer(bd);

		m_Material = new TerrainMaterial("terrain");
		m_Material->GetUniformData().scale = t.scale;
		m_Material->GetUniformData().tileX = t.tileX;
		m_Material->GetUniformData().tileY = t.tileY;
	}

	TerrainLayer::~TerrainLayer()
	{
		delete m_Heightmap;
		delete m_TerrainMesh;
		delete m_TerrainShader;
		delete m_QuadTree;
	}

	void TerrainLayer::Render(LightSetup* ls)
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		uint32* base = (uint32*)m_IBO->Map(MAP_WRITE);
		uint32* left = base;
		m_QuadTree->ConstructIBO(m_Camera, m_Camera->GetPosition(), left, base);
		m_QuadTree->FillCracks(base);
		m_QuadTree->ResetStatus();
		uint32 count = left - base;
		m_IBO->Unmap();
		m_TerrainShader->Bind();
		m_UniformBufferData.mvp = m_Camera->GetViewProjectionMatrix() * m_ModelMatrix;
		m_UniformBufferData.model = m_ModelMatrix;
		m_UniformBufferData.cameraPosition = glm::vec4(m_Camera->GetPosition(), 1);
		m_UniformBuffer->SetData(&m_UniformBufferData, 0, sizeof(TerrainUniformBuffer));
		m_TerrainShader->SetUniformBuffer("TerrainUniformBuffer", m_UniformBuffer);
		m_Material->Bind(m_TerrainShader);
		ls->Bind(m_TerrainShader);
		m_TerrainMesh->RenderStrips(0xffffffff, count);
		//m_TerrainMesh->RenderStrips(0xffffffff);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}

} }