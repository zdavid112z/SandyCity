#pragma once

#include "imageio.h"
#include "modelio.h"

namespace ducky { namespace io {

	struct TerrainMeshData
	{
		ImageData imageData;
		ModelData modelData;
		float scale, tileX, tileY;
		uint32 iboSize = 100000 * sizeof(uint32);
		float distance = 16;

		void ReadFromImage(const ImageData&, float scale, float scaleXZ, const glm::vec2& xzoffset, float tileX, float tileY, float distance = 16, uint32 iboSize = 100000 * sizeof(uint32));
	};

	class TerrainUtils
	{
	public:
		static void CalcNormalTangentBitangent(TerrainMeshData&, uint x, uint y);
	};

} }