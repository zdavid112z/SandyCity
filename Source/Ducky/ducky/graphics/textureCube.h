#pragma once

#include "texture2d.h"

namespace ducky { namespace graphics {

	class TextureCube
	{
	public:
		TextureCube(const io::ImageData& cross, TextureParams);
		TextureCube(io::ImageData* faces, uint numMips, TextureParams);
		~TextureCube();

		void Bind(uint slot = 0);
		static void Unbind(uint slot);

		uint GetWidth() { return m_Width; }
		uint GetHeight() { return m_Height; }
	protected:
		GLuint m_Texture;
		uint m_Width, m_Height;
	};

} }