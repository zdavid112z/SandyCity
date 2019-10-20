#pragma once

#include "ducky_pch.h"
#include "opengl.h"
#include "io\imageio.h"

namespace ducky { namespace graphics {

	struct TextureParams
	{
		TextureParams(bool l = true, bool m = true, bool r = true) :
			linear(l),
			mips(m),
			repeat(r) {}
		bool linear = true;
		bool mips = true;
		bool repeat = true;
	};

	class Texture2D
	{
	public:
		Texture2D(const io::ImageData&, TextureParams);
		~Texture2D();

		void Bind(uint slot = 0);
		static void Unbind(uint slot);

		uint GetWidth() { return m_Width; }
		uint GetHeight() { return m_Height; }
	protected:
		GLuint m_Texture;
		uint m_Width, m_Height;
	};

} }