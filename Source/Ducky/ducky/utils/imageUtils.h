#pragma once

#include <glm\glm.hpp>
#include "ducky_pch.h"
#include "../io/imageio.h"

namespace ducky { namespace utils {

	struct Pixel
	{
		Pixel(uint c = 0) : color(c) {}
		Pixel(byte rr, byte gg, byte bb, byte aa) : r(rr), g(gg), b(bb), a(aa) {}
		Pixel(glm::vec4 v) : 
			r((byte)(v.r * 255.0f)),
			g((byte)(v.g * 255.0f)),
			b((byte)(v.b * 255.0f)),
			a((byte)(v.a * 255.0f)) {}
		glm::vec4 ToVector4()
		{
			glm::vec4 v;
			v.r = (float)r / 255.0f;
			v.g = (float)g / 255.0f;
			v.b = (float)b / 255.0f;
			v.a = (float)a / 255.0f;
			return v;
		}
		union
		{
			uint color;
			struct
			{
				byte r, g, b, a;
			};
		};
	};

	class ImageUtils 
	{
	public:
		static io::ImageData SubImage(const io::ImageData* source, uint x, uint y, uint w, uint h, bool flipX, bool flipY);
		static void StripVCrossIntoFaces(const io::ImageData*, io::ImageData* output);
		static void StripHCrossIntoFaces(const io::ImageData*, io::ImageData* output);
		static void ApplyGammaCorrention(io::ImageData*);
		static io::ImageData RGBToRGBA(const io::ImageData*);
		static io::ImageData Clone(const io::ImageData*);
		static io::ImageData CreateNew(uint width, uint height, io::ImageFormatInternal ifi, io::ImageFormat imf, uint64 color = 0);
		static io::ImageData ResizeLinear(const io::ImageData* org, uint nwidth, uint nheight);
		static io::ImageData ResizeNearest(const io::ImageData* org, uint nwidth, uint nheight);
	};


} }