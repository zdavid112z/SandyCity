#include "ducky_pch.h"
#include "layer2d.h"
#include "utils\stringUtils.h"

namespace ducky { namespace graphics {

	Layer2D::Layer2D() : Layer(GetLayerName(), -1)
	{
		m_InstancesCount = 0;
		m_ZIndices = new float[m_MaxInstances];
		m_Instances = new InstanceData2D[m_MaxInstances];

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_IDAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_IDAO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(InstanceData2D) * m_MaxInstances, nullptr, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData2D), (const void*)(offsetof(InstanceData2D, minmax)));
		glVertexAttribDivisor(0, 1);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData2D), (const void*)(offsetof(InstanceData2D, texture_mask_alpha_limits)));
		glVertexAttribDivisor(1, 1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(InstanceData2D), (const void*)(offsetof(InstanceData2D, borderColor)));
		glVertexAttribDivisor(2, 1);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData2D), (const void*)(offsetof(InstanceData2D, borderLimitsOffset)));
		glVertexAttribDivisor(3, 1);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData2D), (const void*)(offsetof(InstanceData2D, sizePosition)));
		glVertexAttribDivisor(4, 1);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(InstanceData2D), (const void*)(offsetof(InstanceData2D, color1)));
		glVertexAttribDivisor(5, 1);
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(InstanceData2D), (const void*)(offsetof(InstanceData2D, color2)));
		glVertexAttribDivisor(6, 1);
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(InstanceData2D), (const void*)(offsetof(InstanceData2D, color3)));
		glVertexAttribDivisor(7, 1);
		glEnableVertexAttribArray(8);
		glVertexAttribPointer(8, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(InstanceData2D), (const void*)(offsetof(InstanceData2D, color4)));
		glVertexAttribDivisor(8, 1);
		glEnableVertexAttribArray(9);
		glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData2D), (const void*)(offsetof(InstanceData2D, model1)));
		glVertexAttribDivisor(9, 1);
		glEnableVertexAttribArray(10);
		glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData2D), (const void*)(offsetof(InstanceData2D, model2)));
		glVertexAttribDivisor(10, 1);
		glEnableVertexAttribArray(11);
		glVertexAttribPointer(11, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData2D), (const void*)(offsetof(InstanceData2D, model3)));
		glVertexAttribDivisor(11, 1);
		glEnableVertexAttribArray(12);
		glVertexAttribPointer(12, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData2D), (const void*)(offsetof(InstanceData2D, model4)));
		glVertexAttribDivisor(12, 1);

		/*glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint32), indices, GL_STATIC_DRAW);*/

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_Shader = new Shader("assets/shaders/shader2d.vert", "assets/shaders/shader2d.frag");
		int ti[32];
		for (uint i = 0; i < 32; i++)
			ti[i] = i;
		m_Shader->SetUniform1iv("u_Textures", ti, 32);

		BufferDesc bd;
		bd.data = nullptr;
		bd.dynamic = true;
		bd.size = sizeof(UniformBuffer2D);
		bd.type = BUFFER_UNIFORM;
		m_UniformBuffer = new Buffer(bd);

		m_Camera = new Camera(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1000.0f, 1000.0f));
	}

	float Layer2D::AddTexture(Texture2D* t)
	{
		if (t == nullptr)
			return -1;
		for (uint i = 0; i < m_Textures.size(); i++)
		{
			if (m_Textures[i] == t)
				return i;
		}
		m_Textures.push_back(t);
		return m_Textures.size() - 1;
	}

	Layer2D::~Layer2D()
	{

	}

	void Layer2D::DrawImage(const glm::mat4& model, const glm::vec4& sizePosition, Texture2D* texture, float zindex, int spriteW, int spriteH, int posX, int posY, bool flipX, bool flipY, const glm::vec2& alphaLimits, Texture2D* mask, uint32 c1, uint32 c2, uint32 c3, uint32 c4)
	{
		InstanceData2D id;
		id.model = model;
		switch (flipX | (flipY << 1))
		{
		case 0:
			id.minmax = glm::vec4((float)posX / (float)spriteW, (float)posY / (float)spriteH, ((float)posX + 1) / (float)spriteW, ((float)posY + 1) / (float)spriteH);
			break;
		case 1:
			id.minmax = glm::vec4(((float)posX + 1) / (float)spriteW, (float)posY / (float)spriteH, ((float)posX) / (float)spriteW, ((float)posY + 1) / (float)spriteH);
			break;
		case 2:
			id.minmax = glm::vec4((float)posX / (float)spriteW, ((float)posY + 1) / (float)spriteH, ((float)posX + 1) / (float)spriteW, ((float)posY) / (float)spriteH);
			break;
		case 3:
			id.minmax = glm::vec4(((float)posX + 1) / (float)spriteW, ((float)posY + 1) / (float)spriteH, ((float)posX) / (float)spriteW, ((float)posY) / (float)spriteH);
			break;
		}
		id.texture_mask_alpha_limits = glm::vec4(AddTexture(texture), AddTexture(mask), alphaLimits.x, alphaLimits.y);
		id.color1 = c1;
		id.color2 = c2;
		id.color3 = c3;
		id.color4 = c4;
		id.sizePosition = sizePosition;
		AddDrawCall(id, zindex);
	}
	
	void Layer2D::DrawSprite(const glm::mat4& model, const glm::vec4& sizePosition, Sprite* sprite, float zindex, bool flipX, bool flipY, const glm::vec2& alphaLimits, Texture2D* mask, uint32 c1, uint32 c2, uint32 c3, uint32 c4)
	{
		InstanceData2D id;
		id.model = model;
		switch (flipX | (flipY << 1))
		{
		case 0:
			id.minmax = glm::vec4(sprite->texMin.x, sprite->texMin.y, sprite->texMax.x, sprite->texMax.y);
			break;
		case 1:
			id.minmax = glm::vec4(sprite->texMax.x, sprite->texMin.y, sprite->texMin.x, sprite->texMax.y);
			break;
		case 2:
			id.minmax = glm::vec4(sprite->texMin.x, sprite->texMax.y, sprite->texMax.x, sprite->texMin.y);
			break;
		case 3:
			id.minmax = glm::vec4(sprite->texMax.x, sprite->texMax.y, sprite->texMin.x, sprite->texMin.y);
			break;
		}
		id.texture_mask_alpha_limits = glm::vec4(AddTexture(sprite->sheet->GetTexture()), AddTexture(mask), alphaLimits.x, alphaLimits.y);
		id.color1 = c1;
		id.color2 = c2;
		id.color3 = c3;
		id.color4 = c4;
		id.sizePosition = sizePosition;
		AddDrawCall(id, zindex);
	}

	void Layer2D::DrawRect(const glm::mat4& model, const glm::vec4& sizePosition, uint32 c1, uint32 c2, uint32 c3, uint32 c4, float zindex, Texture2D* mask, const glm::vec4& minmax)
	{
		InstanceData2D id;
		id.model = model;
		id.minmax = minmax;
		id.texture_mask_alpha_limits = glm::vec4(-1, AddTexture(mask), 0, 1);
		id.color1 = c1;
		id.color2 = c2;
		id.color3 = c3;
		id.color4 = c4;
		id.sizePosition = sizePosition;
		AddDrawCall(id, zindex);
	}

	void Layer2D::DrawStringSimple(const char* text, io::FontData* fd, Texture2D* fontMap, const glm::vec3& position, float scale, uint color, bool centered, float zindex, const glm::vec2& alphaLimits)
	{
		DrawString(text, fd, fontMap, nullptr, glm::translate(position), scale, color, color, color, color, centered, zindex, alphaLimits);
	}

	void Layer2D::DrawString(const char* text, io::FontData* fd, Texture2D* fontMap, Texture2D* mask, glm::mat4 model, float scale, uint c1, uint c2, uint c3, uint c4, bool centered, float zindex, glm::vec2 alphaLimits, uint borderColor, glm::vec4 borderLimitsOffset, glm::vec3 position)
	{
		if (centered)
		{
			vector<string> lines = utils::StringUtils::SplitString(text, "\n");
			if (lines.size() != 1)
			{
				for (uint i = 0; i < lines.size(); i++)
				{
					DrawString(lines[i].c_str(), fd, fontMap, mask, model, scale, c1, c2, c3, c4, centered, zindex, alphaLimits, borderColor, borderLimitsOffset, position + glm::vec3(0, (fd->lineHeight - fd->paddings[0] - fd->paddings[2]) * scale * (float)i * -1.0f, 0));
				}
				return;
			}
			float l = fd->GetFontMetrics(text, scale);
			position.x -= l / 2.0f;
		}
		float ix = position.x;
		glm::vec3 cursor = position;
		uint prev = NULL;
		while (*text != NULL)
		{
			uint ch = *text;
			if (ch == '\n')
			{
				cursor.x = ix;
				cursor.y -= (fd->lineHeight - fd->paddings[0] - fd->paddings[2]) * scale;
				text++;
				continue;
			}
			io::Character* chData = NULL;
			for (uint i = 0; i < fd->numCharacters; i++)
			{
				if (fd->characters[i].id == ch)
				{
					chData = &fd->characters[i];
					break;
				}
			}

			if (chData != NULL)
				DrawCharacter(*chData, fd, fontMap, mask, model, scale, c1, c2, c3, c4, zindex, alphaLimits, borderColor, borderLimitsOffset, cursor);

			for (uint i = 0; i < fd->numKernings; i++)
			{
				if (fd->kernings[i].first == prev && fd->kernings[i].second == ch)
				{
					cursor.x += fd->kernings[i].amount * scale;
					break;
				}
			}

			cursor.x += (chData->xadvance) * scale;
			text++;
			prev = ch;
		}
	}

	void Layer2D::DrawCharacter(const io::Character& ch, io::FontData* fd, Texture2D* fontMap, Texture2D* mask, glm::mat4 model, float scale, uint c1, uint c2, uint c3, uint c4, float zindex, glm::vec2 alphaLimits, uint borderColor, glm::vec4 borderLimitsOffset, glm::vec3 position)
	{
		InstanceData2D dc;
		dc.texture_mask_alpha_limits = glm::vec4(AddTexture(fontMap), AddTexture(mask), alphaLimits.x, alphaLimits.y);
		dc.color1 = c1;
		dc.color2 = c2;
		dc.color3 = c3;
		dc.color4 = c4;

		glm::vec3 pos = position;
		pos.x += (ch.xoffset) * scale;
		pos.y += (fd->base - (ch.height + ch.yoffset)) * scale;

		pos.x += ch.width / 2.0f * scale;
		pos.y += ch.height / 2.0f * scale;

		glm::vec2 mintc((float)(ch.x) / (float)fd->width, 1.0f - (float)(ch.y + ch.height) / (float)fd->height);
		glm::vec2 maxtc((float)(ch.x + ch.width) / (float)fd->width, 1.0f - (float)(ch.y) / (float)fd->height);
		dc.minmax = glm::vec4(mintc, maxtc);

		//glm::mat4 local = glm::translate(pos) * glm::scale(glm::vec3((ch.width / 2.0f) * scale, (ch.height / 2.0f) * scale, 1.0f));
		dc.sizePosition = glm::vec4(ch.width / 2.0f * scale, ch.height / 2.0f * scale, pos.x, pos.y);
		dc.model = model;
		dc.borderColor = borderColor;
		dc.borderLimitsOffset = borderLimitsOffset;
		AddDrawCall(dc, zindex);
	}

	void Layer2D::AddDrawCall(const InstanceData2D& id, float zindex)
	{
		for (int i = 0; i < m_InstancesCount; i++)
		{
			if (m_ZIndices[i] < zindex)
			{
				for (int j = m_InstancesCount - 1; j >= i; j--)
				{
					m_Instances[j + 1] = m_Instances[j];
					//memcpy(&m_Instances[j + 1], &m_Instances[j], sizeof(InstanceData2D));
					m_ZIndices[j + 1] = m_ZIndices[j];
				}
				m_Instances[i] = id;
				m_ZIndices[i] = zindex;
				m_InstancesCount++;
				return;
			}
		}
		m_Instances[m_InstancesCount] = id;
		m_ZIndices[m_InstancesCount] = zindex;
		m_InstancesCount++;
	}

	void Layer2D::Render(LightSetup* ls)
	{
		if (m_InstancesCount == 0)
			return;

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindBuffer(GL_ARRAY_BUFFER, m_IDAO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(InstanceData2D) * m_InstancesCount, m_Instances);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		m_UniformBufferData.viewProjection = m_Camera->GetViewProjectionMatrix();
		m_UniformBuffer->SetData(&m_UniformBufferData, 0, sizeof(UniformBuffer2D));
		m_Shader->Bind();
		m_Shader->SetUniformBuffer("UniformBuffer2D", m_UniformBuffer);
		for (uint i = 0; i < m_Textures.size(); i++)
			m_Textures[i]->Bind(i);

		glBindVertexArray(m_VAO);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_InstancesCount);
		//glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, m_InstancesCount);
		glBindVertexArray(0);
		m_Textures.clear();
		m_InstancesCount = 0;

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}

} }