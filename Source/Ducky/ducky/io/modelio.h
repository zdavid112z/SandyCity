#pragma once

#include "fileio.h"
#include "glm\glm.hpp"
#include "serializable.h"

namespace ducky { namespace io {

	struct Vertex
	{

		bool operator==(const Vertex& other) const
		{
			return position == other.position && normal == other.normal && texCoord == other.texCoord && bitangent == other.bitangent && tangent == other.tangent;
		}
		glm::vec3 position;
		glm::vec2 texCoord;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};

	enum ModelAttribute
	{
		MA_POSITION = BIT(0),
		MA_UV = BIT(1),
		MA_NORMAL = BIT(2),
		MA_TANGENT = BIT(3),
		MA_BITANGENT = BIT(4),
	};

	enum ModelParserFlags : uint
	{
		MODEL_PARSER_FLIP_UVS = BIT(1)
	};

	class ModelData : public Serializable
	{
	public:
		void Delete();
		void ReadFromFile(const string&, uint flags);
		void Read(IStream*) override;
		void Write(OStream*) override;

		Vertex* vertices = nullptr;
		uint* indices = nullptr;
		uint numVertices;
		uint32 numIndices;
		string name;
		uint attributes;

	};

	class MeshUtils
	{
	public:
		static void CalcNormalTangentBitangent(Vertex& a, Vertex& b, Vertex& c, bool flip = false);
		static glm::vec3 CalcNormal(const Vertex& a, const Vertex& b, const Vertex& c, bool flip = false);
		static void CalcTangentBitangent(const Vertex& a, const Vertex& b, const Vertex& c, glm::vec3& tangent, glm::vec3& bitangent);
	};

} }