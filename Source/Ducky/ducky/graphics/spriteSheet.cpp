#include "ducky_pch.h"
#include "spriteSheet.h"
#include "utils\debug.h"
#include "utils\stringUtils.h"

namespace ducky { namespace graphics {

	SpriteSheet::SpriteSheet(io::ImageData* copy, Texture2D* texture)
	{
		m_ImageData = copy;
		m_Texture = texture;
		utils::Debug::Assert(m_ImageData->iformat == io::IFI_RGBA8);
	}

	SpriteSheet::~SpriteSheet()
	{
		delete m_Texture;
	}

	byte SpriteSheet::GetPixelAlpha(uint x, uint y)
	{
		return ((((uint*)(m_ImageData->data))[x + y * m_ImageData->width]) & 0xff000000) >> 24;
	}

	void SpriteSheet::NullPixel(uint x, uint y)
	{
		((uint*)(m_ImageData->data))[x + y * m_ImageData->width] = 0;
	}

	glm::u16vec4 SpriteSheet::GetMinMax(uint x, uint y)
	{
		m_Stack.clear();
		uint r = 0;
		glm::u16vec4 minmax(x, y, x, y);
		m_Stack.push_back(glm::u16vec2(x, y));
		NullPixel(x, y);
		while (m_Stack.size() > r)
		{
			glm::u16vec2 c = m_Stack[r];
			if (c.x > 0)
			{
				if (GetPixelAlpha(c.x - 1, c.y) != 0)
				{
					m_Stack.push_back(glm::u16vec2(c.x - 1, c.y));
					NullPixel(c.x - 1, c.y);
					if (minmax.x > c.x - 1)
						minmax.x = c.x - 1;
				}
			}
			if (c.y > 0)
			{
				if (GetPixelAlpha(c.x, c.y - 1) != 0)
				{
					m_Stack.push_back(glm::u16vec2(c.x, c.y - 1));
					NullPixel(c.x, c.y - 1);
					if (minmax.y > c.y - 1)
						minmax.y = c.y - 1;
				}
			}
			if (c.x < m_ImageData->width - 1)
			{
				if (GetPixelAlpha(c.x + 1, c.y) != 0)
				{
					m_Stack.push_back(glm::u16vec2(c.x + 1, c.y));
					NullPixel(c.x + 1, c.y);
					if (minmax.z < c.x + 1)
						minmax.z = c.x + 1;
				}
			}
			if (c.y < m_ImageData->height - 1)
			{
				if (GetPixelAlpha(c.x, c.y + 1) != 0)
				{
					m_Stack.push_back(glm::u16vec2(c.x, c.y + 1));
					NullPixel(c.x, c.y + 1);
					if (minmax.w < c.y + 1)
						minmax.w = c.y + 1;
				}
			}
			r++;
		}
		return minmax;
	}

	void SpriteSheet::GenerateAuto(int padding, uint minSize)
	{
		m_Sprites.clear();
		for (int y = m_ImageData->height - 1; y >= 0; y--)
		{
			for (uint x = 0; x < m_ImageData->width; x++)
			{
				if (GetPixelAlpha(x, y) != 0)
				{
					glm::u16vec4 minmax = GetMinMax(x, y);
					Sprite s;
					s.sheet = this;
					s.texMin = glm::vec2((int)minmax.x - padding, (int)minmax.y - padding) / glm::vec2(m_ImageData->width, m_ImageData->height);
					s.texMax = glm::vec2((int)minmax.z + padding, (int)minmax.w + padding) / glm::vec2(m_ImageData->width, m_ImageData->height);
					s.name = "generatedAuto_" + utils::StringUtils::ToString(m_Sprites.size(), 10);
					m_Sprites.push_back(s);
				}
			}
		}
	}

	void SpriteSheet::GenerateGrid(uint width, uint height, bool topDown)
	{
		m_Sprites.clear();
		for (uint i = 0; i < m_ImageData->height / height; i++)
		{
			for (uint j = 0; j < m_ImageData->width / width; j++)
			{
				bool hasAlpha = false;
				for (uint y = i * height; y < (i + 1) * height; y++)
				{
					for (uint x = j * width; x < (j + 1) * width; x++)
					{
						if (topDown)
						{
							if (GetPixelAlpha(x, m_ImageData->height - y - 1) != 0)
							{
								hasAlpha = true;
								break;
							}
						}
						else
						{
							if (GetPixelAlpha(x, y) != 0)
							{
								hasAlpha = true;
								break;
							}
						}
					}
					if (hasAlpha)
						break;
				}

				if (hasAlpha)
				{
					Sprite s;
					s.sheet = this;
					s.name = "generatedGrid_" + utils::StringUtils::ToString(m_Sprites.size(), 10);
					if (topDown)
					{
						s.texMin = glm::vec2(
							float(j * width) / (float)m_ImageData->width,
							float(m_ImageData->height - (i + 1) * height) / (float)m_ImageData->height);
							
						s.texMax = glm::vec2(
							float((j + 1) * width) / (float)m_ImageData->width,
							float(m_ImageData->height - i * height) / (float)m_ImageData->height);
					}
					else
					{
						s.texMin = glm::vec2(
							float(j * width) / (float)m_ImageData->width,
							float(i * height) / (float)m_ImageData->height);
						s.texMax = glm::vec2(
							float((j + 1) * width) / (float)m_ImageData->width,
							float((i + 1) * height) / (float)m_ImageData->height);
					}
					m_Sprites.push_back(s);
				}
			}
		}
	}

	void SpriteSheet::Read(io::IStream& in)
	{
		uint16 size;
		in.ReadUShort(size);
		m_Sprites.reserve(size);
		for (uint i = 0; i < size; i++)
		{
			Sprite s;
			s.sheet = this;
			in.ReadString(s.name);
			in.ReadBytes((byte*)&s.texMin, sizeof(glm::vec2));
			in.ReadBytes((byte*)&s.texMax, sizeof(glm::vec2));
			m_Sprites.push_back(s);
		}
	}

	void SpriteSheet::Write(io::OStream& out) const
	{
		out.WriteUShort(m_Sprites.size());
		for (uint i = 0; i < m_Sprites.size(); i++)
		{
			out.WriteString(m_Sprites[i].name);
			out.WriteBytes((byte*)&m_Sprites[i].texMin, sizeof(glm::vec2));
			out.WriteBytes((byte*)&m_Sprites[i].texMax, sizeof(glm::vec2));
		}
	}
	
	Sprite* SpriteSheet::Rename(const string& oldName, const string& newName)
	{
		Sprite* s = GetSprite(oldName);
		if (s != nullptr)
			s->name = newName;
		return s;
	}

	Sprite* SpriteSheet::AddSprite(const Sprite& s)
	{
		m_Sprites.push_back(s);
		return &m_Sprites[m_Sprites.size() - 1];
	}

	Texture2D* SpriteSheet::GetTexture() const
	{
		return m_Texture;
	}

	Sprite* SpriteSheet::GetSprite(const string& name)
	{
		for (uint i = 0; i < m_Sprites.size(); i++)
			if (m_Sprites[i].name == name)
				return &m_Sprites[i];
		return nullptr;
	}

	Sprite* SpriteSheet::GetSprite(uint index)
	{
		return &m_Sprites[index];
	}

	uint SpriteSheet::GetNumSprites() const
	{
		return m_Sprites.size();
	}

} }