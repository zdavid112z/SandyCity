#pragma once

#include "../buffer.h"
#include "../camera.h"
#include "io\terrainio.h"

namespace ducky { namespace graphics {
	
	class TerrainQuadTree
	{
	public:
		TerrainQuadTree(const io::TerrainMeshData& td, uint x, uint y, uint lod, float dist, TerrainQuadTree* parent, TerrainQuadTree* root);
		~TerrainQuadTree();

		void ResetStatus();
		void ConstructIBO(Camera* cam, const glm::vec3& campos, uint*& data, uint* base);
		void AddToIBO(uint*& data, uint* base);
		bool ShouldRenderLOD(const glm::vec3& campos);
		void FillCracks(uint* data);
		TerrainQuadTree* Trace(int x, int y, int lod, int twoAtLod);
		void SetNull(uint* data);
		uint32 GetIBOIndex(int x, int y);
	private:
		float m_Distance;
		glm::vec2 m_Coords;
		glm::vec2 m_TerrainSize;
		glm::vec3 m_Center;
		uint m_LOD;
		TerrainQuadTree* m_Root;
		TerrainQuadTree* m_Parent;
		TerrainQuadTree* m_Children[4];
		float m_2AtLOD;
		float m_DistMultiplier;
		// 0 = unvisited, 1 = visited, 2 = rendered
		byte m_Status = 0;
		uint m_Position;
	};

} }