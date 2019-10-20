#include "ducky_pch.h"
#include "texture2d.h"

namespace ducky { namespace graphics {

	Texture2D::Texture2D(const io::ImageData& id, TextureParams tp)
	{
		m_Width = id.width;
		m_Height = id.height;
		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tp.repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tp.repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tp.linear ? GL_LINEAR : GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tp.linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tp.mips ? (tp.linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST) : (tp.linear ? GL_LINEAR : GL_NEAREST));

		glTexImage2D(GL_TEXTURE_2D, 0, io::ImageData::GetAPIImageFormatInternal(id.iformat), id.width, id.height, 0, io::ImageData::GetAPIImageFormat(id.format), GL_UNSIGNED_BYTE, id.data);
		if (tp.mips)
			glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_Texture);
	}

	void Texture2D::Bind(uint slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
	}

	void Texture2D::Unbind(uint slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

} }