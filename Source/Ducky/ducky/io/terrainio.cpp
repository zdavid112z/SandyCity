#include "ducky_pch.h"
#include "terrainio.h"

namespace ducky { namespace io {

	void TerrainUtils::CalcNormalTangentBitangent(TerrainMeshData& t, uint x, uint y)
	{
		
	}

	void TerrainMeshData::ReadFromImage(const ImageData& id, float scale, float scaleXZ, const glm::vec2& xzoffset, float tileX, float tileY, float distance, uint32 iboSize)
	{
		imageData = id;
		this->distance = distance;
		this->iboSize = iboSize;
		this->scale = scale;
		this->tileX = tileX * id.width;
		this->tileY = tileY * id.height;
		modelData.numVertices = id.width * id.height;
		modelData.numIndices = (id.width * 2 + 1) * (id.height - 1);
		modelData.vertices = new Vertex[modelData.numVertices];
		//modelData.indices = new uint[modelData.numIndices];

		/*uint next = 0;
		for (uint y = 0; y < id.height - 1; y++)
		{
			for (uint x = 0; x < id.width; x++)
			{
				modelData.indices[next++] = (x) + (y) * id.width;
				modelData.indices[next++] = (x) + (y + 1)* id.width;
			}
			modelData.indices[next++] = 0xffffffff;
		}*/

		for (uint y = 0; y < id.height; y++)
		{
			for (uint x = 0; x < id.width; x++)
			{
				uint bytePosition = y * id.pitch + x * id.bpp / 8;
				float height = 0;
				if (id.bpp == 8)
					height = (float)id.data[bytePosition] / 255.0f;
				else if(id.bpp == 16)
					height = (float)(*(uint16*)&id.data[bytePosition]) / 65535.0f;
				modelData.vertices[x + y * id.width].position = glm::vec3(xzoffset.x + x * scaleXZ, height * scale, xzoffset.y + y * scaleXZ);
				modelData.vertices[x + y * id.width].texCoord = glm::vec2(x * tileX, y * tileY);
			}
		}

		for (uint y = 0; y < id.height; y++)
		{
			for (uint x = 0; x < id.width; x++)
			{
				glm::vec3 normal = glm::vec3(0);
				glm::vec3 tangent = glm::vec3(0);
				glm::vec3 bitangent = glm::vec3(0);
				glm::vec3 at, ab;
				uint num = 0;
				if (x > 0 && y > 0)
				{
					normal += MeshUtils::CalcNormal(modelData.vertices[(x - 1) + y * id.width], modelData.vertices[(x)+(y - 1)* id.width], modelData.vertices[(x)+(y)* id.width]);
					MeshUtils::CalcTangentBitangent(modelData.vertices[(x - 1) + y * id.width], modelData.vertices[(x)+(y - 1)* id.width], modelData.vertices[(x)+(y)* id.width], at, ab);
					tangent += at;
					bitangent += ab;
					num++;
				}

				if (x < id.width - 1 && y < id.height - 1)
				{
					normal += MeshUtils::CalcNormal(modelData.vertices[(x)+(y)* id.width], modelData.vertices[(x + 1) + (y)* id.width], modelData.vertices[(x)+(y + 1)* id.width]);
					MeshUtils::CalcTangentBitangent(modelData.vertices[(x)+(y)* id.width], modelData.vertices[(x + 1) + (y)* id.width], modelData.vertices[(x)+(y + 1)* id.width], at, ab);
					tangent += at;
					bitangent += ab;
					num++;
				}
				if (x > 0 && y < id.height - 1)
				{
					normal += MeshUtils::CalcNormal(modelData.vertices[(x - 1) + (y)* id.width], modelData.vertices[(x)+(y) * id.width], modelData.vertices[(x - 1) + (y + 1)* id.width]);
					MeshUtils::CalcTangentBitangent(modelData.vertices[(x - 1) + (y)* id.width], modelData.vertices[(x)+(y) * id.width], modelData.vertices[(x - 1) + (y + 1)* id.width], at, ab);
					tangent += at;
					bitangent += ab;

					normal += MeshUtils::CalcNormal(modelData.vertices[(x - 1) + (y + 1)* id.width], modelData.vertices[(x)+(y)* id.width], modelData.vertices[(x)+(y + 1)* id.width]);
					MeshUtils::CalcTangentBitangent(modelData.vertices[(x - 1) + (y + 1)* id.width], modelData.vertices[(x)+(y)* id.width], modelData.vertices[(x)+(y + 1)* id.width], at, ab);
					tangent += at;
					bitangent += ab;

					num += 2;
				}
				if (x < id.width - 1 && y > 0)
				{
					normal += MeshUtils::CalcNormal(modelData.vertices[(x)+(y)* id.width], modelData.vertices[(x)+(y - 1)* id.width], modelData.vertices[(x + 1) + (y - 1)* id.width]);
					MeshUtils::CalcTangentBitangent(modelData.vertices[(x)+(y)* id.width], modelData.vertices[(x)+(y - 1)* id.width], modelData.vertices[(x + 1) + (y - 1)* id.width], at, ab);
					tangent += at;
					bitangent += ab;

					normal += MeshUtils::CalcNormal(modelData.vertices[(x)+(y)* id.width], modelData.vertices[(x + 1) + (y - 1)* id.width], modelData.vertices[(x + 1) + (y)* id.width]);
					MeshUtils::CalcTangentBitangent(modelData.vertices[(x)+(y)* id.width], modelData.vertices[(x + 1) + (y - 1)* id.width], modelData.vertices[(x + 1) + (y)* id.width], at, ab);
					tangent += at;
					bitangent += ab;

					num += 2;
				}

				normal /= num;
				tangent /= num;
				bitangent /= num;
				modelData.vertices[(x)+(y)* id.width].normal = normal;
				modelData.vertices[(x)+(y)* id.width].tangent = tangent;
				modelData.vertices[(x)+(y)* id.width].bitangent = bitangent;
			}
		}

	}

} }