#pragma once

#include "io\terrainio.h"
#include "layer3d.h"
#include "graphics\transform.h"
#include "graphics\materials\terrainMaterial.h"
#include "terrainQuadTree.h"

namespace ducky { namespace graphics {

	struct TerrainUniformBuffer
	{
		glm::mat4 mvp;
		glm::mat4 model;
		glm::vec4 cameraPosition;
	};

	class TerrainLayer : public Layer
	{
	public:
		TerrainLayer(const io::TerrainMeshData&);
		virtual ~TerrainLayer();

		void Render(LightSetup*);
		void SetCamera(Camera* camera) { m_Camera = camera; }
		void SetModelMatrix(const glm::mat4& m) { m_ModelMatrix = m; }

		static string GetLayerName() { return "Terrain"; }
		TerrainMaterial* GetMaterial() { return m_Material; }
	private:
		TerrainQuadTree* m_QuadTree;
		TerrainMaterial* m_Material;
		Camera* m_Camera;
		glm::mat4 m_ModelMatrix;
		io::TerrainMeshData m_TerrainData;
		Texture2D* m_Heightmap;
		Mesh* m_TerrainMesh;
		Shader* m_TerrainShader;
		Buffer* m_UniformBuffer;
		Buffer* m_IBO;
		TerrainUniformBuffer m_UniformBufferData;
	};

} }