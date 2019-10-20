#pragma once

#include "ducky.h"

using namespace ducky;
using namespace graphics;

struct LDUniformBuffer
{
	LDUniformBuffer(float f = 0, const glm::vec4& c = glm::vec4(1, 1, 1, 1)) :
		color(c), lightFactor(f) {}
	glm::vec4 color = glm::vec4(1, 1, 1, 1);
	float lightFactor = 0;
	glm::vec3 padding;
};

class LDMaterial : public Material
{
public:
	LDMaterial(const string& name, Texture2D* texture, const LDUniformBuffer& buffer) : Material(name) {
		BufferDesc bd;
		bd.data = (void*)&buffer;
		bd.dynamic = true;
		bd.size = sizeof(LDUniformBuffer);
		bd.type = BUFFER_UNIFORM;
		m_UniformBuffer = new Buffer(bd);
		m_Texture = texture;
	}

	LDMaterial(const string& name) : Material(name) {
		BufferDesc bd;
		bd.data = (void*)&m_BufferData;
		bd.dynamic = true;
		bd.size = sizeof(LDUniformBuffer);
		bd.type = BUFFER_UNIFORM;
		m_UniformBuffer = new Buffer(bd);
	}

	void Bind(Shader* shader) override
	{
		shader->SetUniformBuffer("MaterialUniformBuffer", m_UniformBuffer);
		if (m_Texture != nullptr)
			m_Texture->Bind(0);
	}

	void Unbind(Shader* shader) override
	{
		if (m_Texture != nullptr)
			m_Texture->Unbind(0);
	}

	LDUniformBuffer* GetUniformData() { return &m_BufferData; }
	void UpdateBuffer() { m_UniformBuffer->SetData(&m_BufferData, 0, sizeof(LDUniformBuffer)); }

	void SetTexture(Texture2D* t) { m_Texture = t; }
	Texture2D* GetTexture() { return m_Texture; }
private:
	Buffer* m_UniformBuffer;
	LDUniformBuffer m_BufferData;
	Texture2D* m_Texture;
};