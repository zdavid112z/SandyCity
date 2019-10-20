#include "ducky_pch.h"
#include "modelio.h"
#include "fileio.h"
#include "../utils/debug.h"
#ifndef DUCKY_DISABLE_ASSIMP
#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"
#include "assimp\scene.h"
#endif

namespace ducky { namespace io {

	void ModelData::Delete()
	{
		if (vertices)
			delete vertices;
		if (indices)
			delete indices;
	}

	void ModelData::Read(IStream* in)
	{
		if (vertices)
			delete vertices;
		if (indices)
			delete indices;

		in->ReadString(name);

		in->ReadUInt(attributes);

		in->ReadUInt(numVertices);
		vertices = new Vertex[numVertices];
		in->ReadBytes((byte*)vertices, sizeof(Vertex) * numVertices);

		in->ReadUInt(numIndices);
		indices = new uint[numIndices];
		in->ReadBytes((byte*)indices, sizeof(uint) * numIndices);
	}

	void ModelData::Write(OStream* out)
	{
		out->WriteString(name);
		out->WriteUInt(attributes);
		out->WriteUInt(numVertices);
		out->WriteBytes((byte*)vertices, numVertices * sizeof(Vertex));
		out->WriteUInt(numIndices);
		out->WriteBytes((byte*)indices, numIndices * sizeof(uint));
	}

	void ModelData::ReadFromFile(const string& cpath, uint flags)
	{
#ifndef DUCKY_DISABLE_ASSIMP
		Assimp::Importer importer;

		uint fs = aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_JoinIdenticalVertices |
			aiProcess_CalcTangentSpace;
		if ((flags & MODEL_PARSER_FLIP_UVS) != 0)
			fs |= aiProcess_FlipUVs;
		string path = FileUtils::ProcessPath(cpath);
		const aiScene* scene = importer.ReadFile(path, fs);

		if (!scene)
		{
			Debug::Printer << FATAL << "Could not load mesh " << path.c_str() << " !\n" << VERB;
			Debug::Assert(false);

		}

		const aiMesh* model = scene->mMeshes[0];

		//vector<Vertex> vertices;
		//vector<unsigned int> indices;
		numIndices = model->mNumFaces * 3;
		numVertices = model->mNumVertices;
		indices = new uint[numIndices];
		vertices = new Vertex[numVertices];

		/*glm::mat4 root;
		
		root[0][0] = scene->mRootNode->mTransformation.a1;
		root[0][1] = scene->mRootNode->mTransformation.a2;
		root[0][2] = scene->mRootNode->mTransformation.a3;
		root[0][3] = scene->mRootNode->mTransformation.a4;

		root[1][0] = scene->mRootNode->mTransformation.b1;
		root[1][1] = scene->mRootNode->mTransformation.b2;
		root[1][2] = scene->mRootNode->mTransformation.b3;
		root[1][3] = scene->mRootNode->mTransformation.b4;

		root[2][0] = scene->mRootNode->mTransformation.c1;
		root[2][1] = scene->mRootNode->mTransformation.c2;
		root[2][2] = scene->mRootNode->mTransformation.c3;
		root[2][3] = scene->mRootNode->mTransformation.c4;

		root[3][0] = scene->mRootNode->mTransformation.d1;
		root[3][1] = scene->mRootNode->mTransformation.d2;
		root[3][2] = scene->mRootNode->mTransformation.d3;
		root[3][3] = scene->mRootNode->mTransformation.d4;
		*/
		//root = Matrix4::Transpose(root);

		const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
		for (unsigned int i = 0; i < model->mNumVertices; i++)
		{
			const aiVector3D pos = model->mVertices[i];
			const aiVector3D normal = model->mNormals[i];
			const aiVector3D texCoord = model->HasTextureCoords(0) ? model->mTextureCoords[0][i] : aiZeroVector;
			const aiVector3D tangent = model->mTangents[i];
			const aiVector3D bitangent = model->mBitangents[i];

			Vertex v;
			v.position = glm::vec3(pos.x, pos.y, pos.z);
			v.texCoord = glm::vec2(texCoord.x, texCoord.y);
			v.normal = glm::vec3(normal.x, normal.y, normal.z);
			v.tangent = glm::vec3(tangent.x, tangent.y, tangent.z);
			v.bitangent = glm::vec3(bitangent.x, bitangent.y, bitangent.z);
			vertices[i] = v;
		}

		for (unsigned int i = 0; i < model->mNumFaces; i++)
		{
			const aiFace& face = model->mFaces[i];
			Debug::Assert(face.mNumIndices == 3, "Model triangulation failed!");
			indices[i * 3 + 0] = face.mIndices[0];
			indices[i * 3 + 1] = face.mIndices[1];
			indices[i * 3 + 2] = face.mIndices[2];
		}

		attributes = MA_POSITION | MA_UV | MA_NORMAL | MA_TANGENT | MA_BITANGENT;
		name = string(model->mName.C_Str());
#endif
	}

	void MeshUtils::CalcNormalTangentBitangent(Vertex& a, Vertex& b, Vertex& c, bool flip)
	{
		glm::vec3 normal = CalcNormal(a, b, c, flip);
		a.normal = normal;
		b.normal = normal;
		c.normal = normal;
		glm::vec3 tangent, bitangent;
		CalcTangentBitangent(a, b, c, tangent, bitangent);
		a.tangent = tangent;
		b.tangent = tangent;
		c.tangent = tangent;
		a.bitangent = bitangent;
		b.bitangent = bitangent;
		c.bitangent = bitangent;
	}

	glm::vec3 MeshUtils::CalcNormal(const Vertex& a, const Vertex& b, const Vertex& c, bool flip)
	{
		glm::vec3 normal = glm::normalize(glm::cross(b.position - a.position, c.position - a.position));
		if (flip)
			return -normal;
		return normal;
	}

	void MeshUtils::CalcTangentBitangent(const Vertex& a, const Vertex& b, const Vertex& c, glm::vec3& tangent, glm::vec3& bitangent)
	{
		glm::vec3 deltaPos1 = b.position - a.position;
		glm::vec3 deltaPos2 = c.position - a.position;

		glm::vec2 deltaUV1 = b.texCoord - a.texCoord;
		glm::vec2 deltaUV2 = c.texCoord - a.texCoord;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
	}

} }