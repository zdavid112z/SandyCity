#include "ducky_pch.h"
#include "textureCube.h"
#include "utils\imageUtils.h"

namespace ducky { namespace graphics {

	TextureCube::TextureCube(const io::ImageData& id, TextureParams tp)
	{
		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, tp.repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, tp.repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, tp.repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, tp.linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, tp.mips ? (tp.linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST) : (tp.linear ? GL_LINEAR : GL_NEAREST));
		
		io::ImageData* faces = new io::ImageData[6];
		utils::ImageUtils::StripVCrossIntoFaces(&id, faces);

		m_Width = faces[0].width;
		m_Height = faces[0].height;

		for (uint i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, io::ImageData::GetAPIImageFormatInternal(faces[i].iformat), faces[i].width, faces[i].height, 0, io::ImageData::GetAPIImageFormat(faces[i].format), GL_UNSIGNED_BYTE, faces[i].data);
		}
		if (tp.mips)
			glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	TextureCube::TextureCube(io::ImageData* crossMips, uint numMips, TextureParams tp)
	{
		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, tp.repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, tp.repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, tp.repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, tp.linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, tp.mips ? (tp.linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST) : (tp.linear ? GL_LINEAR : GL_NEAREST));


		m_Width = crossMips[0].width;
		m_Height = crossMips[0].height;

		uint next = 0;
		for (uint m = 0; m < numMips; m++) 
		{
			for (uint i = 0; i < 6; i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m, io::ImageData::GetAPIImageFormatInternal(crossMips[next].iformat), crossMips[next].width, crossMips[next].height, 0, io::ImageData::GetAPIImageFormat(crossMips[next].format), GL_UNSIGNED_BYTE, crossMips[next].data);
				next++;
			}
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	TextureCube::~TextureCube()
	{
		glDeleteTextures(1, &m_Texture);
	}

	void TextureCube::Bind(uint slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);
	}

	void TextureCube::Unbind(uint slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

} }