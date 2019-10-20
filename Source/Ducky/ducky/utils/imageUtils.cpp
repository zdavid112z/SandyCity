#include "ducky_pch.h"
#include "debug.h"
#include "imageUtils.h"

using namespace ducky::io;


namespace ducky { namespace utils {

	ImageData ImageUtils::SubImage(const ImageData* source, uint x, uint y, uint w, uint h, bool flipX, bool flipY)
	{
		Debug::Assert(x >= 0, "SubImage with negative x called!\n");
		Debug::Assert(y >= 0, "SubImage with negative y called!\n");
		Debug::Assert(x < source->width, "SubImage with out of bounds x called!\n");
		Debug::Assert(y < source->height, "SubImage with out of bounds y called!\n");
		Debug::Assert(x + w >= 0, "SubImage with negative final coordinates on the x axis called!\n");
		Debug::Assert(y + h >= 0, "SubImage with negative final coordinates on the y axis called!\n");
		Debug::Assert(x + w <= source->width, "SubImage with out of bounds width called!\n");
		Debug::Assert(y + h <= source->height, "SubImage with out of bounds height called!\n");

		ImageData res;
		res.width = w;
		res.height = h;
		res.bpp = source->bpp;
		res.pitch = w * (source->bpp / 8);
		res.format = source->format;
		res.iformat = source->iformat;
		res.size = w * h * source->bpp / 8;
		res.data = new byte[res.size];
		uint psize = source->bpp / 8;

		uint resRowSize = source->bpp / 8 * w;
		uint srcRowOffset = source->bpp / 8 * x;
		uint srcRowSize = source->bpp / 8 * source->width;
		if (!flipX)
		{
			if (flipY)
				for (uint yy = 0; yy < h; yy++)
					memcpy(res.data + (h - yy - 1) * resRowSize, source->data + srcRowSize * (yy + y) + srcRowOffset, resRowSize);
			else
				for (uint yy = 0; yy < h; yy++)
					memcpy(res.data + yy * resRowSize, source->data + srcRowSize * (yy + y) + srcRowOffset, resRowSize);
		}
		else
		{
			if (flipY)
			{
				for (uint yy = 0; yy < h; yy++)
				{
					for (uint xx = 0; xx < w; xx++)
					{
						memcpy(res.data + (h - yy - 1) * resRowSize + (w - xx - 1) * psize, source->data + srcRowSize * (yy + y) + srcRowOffset + xx * psize, psize);
					}
				}
			}
			else
			{
				for (uint yy = 0; yy < h; yy++)
				{
					for (uint xx = 0; xx < w; xx++)
					{
						memcpy(res.data + yy * resRowSize + (w - xx - 1 ) * psize, source->data + srcRowSize * (yy + y) + srcRowOffset + xx * psize, psize);
					}
				}
			}
		}
		return res;
	}

	void ImageUtils::StripVCrossIntoFaces(const ImageData* image, io::ImageData* output)
	{
		//x+ right
		//x- left
		//y+ top
		//y- bottom-middle
		//z+ top-middle
		//z- bottom (flipped required)

		Debug::Assert(image->width / 3 == image->height / 4, "Aspect ratio 3:4 required for stripping a vertical cross cubemap!\n");
		uint step = image->width / 3;


		//output[3] = SubImage(image, step * 1, step * 3, step, step, false, true);
		//output[1] = SubImage(image, step * 0, step * 2, step, step, false, true);
		//output[0] = SubImage(image, step * 1, step * 2, step, step, false, true);
		//output[4] = SubImage(image, step * 2, step * 2, step, step, false, true);
		//output[2] = SubImage(image, step * 1, step * 1, step, step, false, true);
		//output[5] = SubImage(image, step * 1, step * 0, step, step, true, false);

		output[0] = SubImage(image, step * 2, step * 2, step, step, false, true);
		output[1] = SubImage(image, step * 0, step * 2, step, step, false, true);
		output[2] = SubImage(image, step * 1, step * 3, step, step, false, true);
		output[3] = SubImage(image, step * 1, step * 1, step, step, false, true);
		output[4] = SubImage(image, step * 1, step * 2, step, step, false, true);
		output[5] = SubImage(image, step * 1, step * 0, step, step, true, false);

		/*

		output[0] = SubImage(image, step * 2, step * 2, step, step, false, true);
		output[1] = SubImage(image, step * 0, step * 2, step, step, false, true);
		output[2] = SubImage(image, step * 1, step * 3, step, step, false, true);
		output[3] = SubImage(image, step * 1, step * 1, step, step, false, true);
		output[4] = SubImage(image, step * 1, step * 2, step, step, false, true);
		output[5] = SubImage(image, step * 1, step * 0, step, step, true, false);
		*/
	}

	void ImageUtils::StripHCrossIntoFaces(const ImageData* image, io::ImageData* output)
	{
		//x+ right
		//x- left
		//y+ top
		//y- bottom-middle
		//z+ top-middle
		//z- bottom (flipped required)

		Debug::Assert(image->width / 3 == image->height / 4, "Aspect ratio 3:4 required for stripping a vertical cross cubemap!\n");
		uint step = image->width / 3;

		output[0] = SubImage(image, step * 2, step * 1, step, step, false, false);
		output[1] = SubImage(image, step * 0, step * 1, step, step, false, false);
		output[2] = SubImage(image, step * 1, step * 0, step, step, false, false);
		output[3] = SubImage(image, step * 1, step * 2, step, step, false, false);
		output[4] = SubImage(image, step * 1, step * 1, step, step, false, false);
		output[5] = SubImage(image, step * 3, step * 1, step, step, false, false);
	}

	io::ImageData ImageUtils::CreateNew(uint width, uint height, io::ImageFormatInternal ifi, io::ImageFormat imf, uint64 color)
	{
		ImageData idata;
		idata.width = width;
		idata.height = height;
		idata.iformat = ifi;
		idata.format = imf;
		idata.bpp = ImageData::GetImageFormatInternalBPP(ifi);
		// TODO: Padding bytes added to some formats
		idata.size = width * height * (idata.bpp / 8);
		idata.pitch = width * (idata.bpp / 8);
		idata.data = new byte[idata.size];
		for (uint y = 0; y < idata.height; y ++)
		{
			for (uint x = 0; x < idata.width; x++)
			{
				memcpy(idata.data + x * idata.bpp / 8 + y * idata.pitch, &color, idata.bpp / 8);
			}
		}
		return idata;
	}

	io::ImageData ImageUtils::ResizeLinear(const io::ImageData* org, uint nwidth, uint nheight)
	{
		ImageData res;
		res.bpp = org->bpp;
		res.format = org->format;
		res.iformat = org->iformat;
		res.width = nwidth;
		res.height = nheight;
		res.pitch = nwidth * (org->bpp / 8);
		res.size = nwidth * nheight * org->bpp / 8;
		res.data = new byte[res.size];
		uint b = org->bpp / 8;
		uint avg;
		for (uint y = 0; y < nheight; y++)
		{
			for (uint x = 0; x < nwidth; x++)
			{
				float xx = x / (float)nwidth;
				float yy = y / (float)nheight;
				uint ox = uint(xx * (float)org->width);
				uint oy = uint(yy * (float)org->height);
				//Pixel p1(org->data[(ox + 0) + (oy + 0) * org->width]);
				//Pixel p2(org->data[(ox + 1) + (oy + 0) * org->width]);
				//Pixel p3(org->data[(ox + 0) + (oy + 1) * org->width]);
				//Pixel p4(org->data[(ox + 1) + (oy + 1) * org->width]);
				//Pixel r;
				//r.r = (p1.r + p2.r + p3.r + p4.r) / 4;
				//r.g = (p1.g + p2.g + p3.g + p4.g) / 4;
				//r.b = (p1.b + p2.b + p3.b + p4.b) / 4;
				//r.a = (p1.a + p2.a + p3.a + p4.a) / 4;
				for (uint i = 0; i < b; i++)
				{
					avg  = org->data[((ox + 0) + (oy + 0) * org->width) * b + i];
					avg += org->data[((ox + 1) + (oy + 0) * org->width) * b + i];
					avg += org->data[((ox + 0) + (oy + 1) * org->width) * b + i];
					avg += org->data[((ox + 1) + (oy + 1) * org->width) * b + i];
					avg /= 4;
					res.data[(x + y * res.width) * b + i] = avg;
				}
			}
		}
		return res;
	}

	io::ImageData ImageUtils::ResizeNearest(const io::ImageData* org, uint nwidth, uint nheight)
	{
		ImageData res;
		res.bpp = org->bpp;
		res.format = org->format;
		res.iformat = org->iformat;
		res.width = nwidth;
		res.height = nheight;
		res.pitch = nwidth * (org->bpp / 8);
		res.size = nwidth * nheight * org->bpp / 8;
		res.data = new byte[res.size];
		uint b = org->bpp / 8;
		for (uint y = 0; y < nheight; y++)
		{
			for (uint x = 0; x < nwidth; x++)
			{
				float xx = x / (float)nwidth;
				float yy = y / (float)nheight;
				uint ox = uint(xx * (float)org->width);
				uint oy = uint(yy * (float)org->height);
				
				//Pixel p1(org->data[(ox + 0) + (oy + 0) * org->width]);
				//Pixel p2(org->data[(ox + 1) + (oy + 0) * org->width]);
				//Pixel p3(org->data[(ox + 0) + (oy + 1) * org->width]);
				//Pixel p4(org->data[(ox + 1) + (oy + 1) * org->width]);
				//Pixel r;
				//r.r = (p1.r + p2.r + p3.r + p4.r) / 4;
				//r.g = (p1.g + p2.g + p3.g + p4.g) / 4;
				//r.b = (p1.b + p2.b + p3.b + p4.b) / 4;
				//r.a = (p1.a + p2.a + p3.a + p4.a) / 4;
				for (uint i = 0; i < b; i++)
					res.data[(x + y * res.width) * b + i] = org->data[(ox + oy * org->width) * b + i];
			}
		}
		return res;
	}

	io::ImageData ImageUtils::RGBToRGBA(const io::ImageData* src)
	{
		if (src->iformat == IFI_RGBA8)
			return Clone(src);
		Debug::Assert(src->format == IFI_RGB8);
		ImageData newImage;
		newImage.data = new byte[src->width * src->height * 4];
		newImage.size = src->width * src->height * 4;
		newImage.iformat = IFI_RGBA8;
		newImage.format = IF_RGBA;
		newImage.bpp = 32;
		newImage.width = src->width;
		newImage.height = src->height;
		newImage.pitch = newImage.width * 4;
		for (uint i = 0; i < newImage.width * newImage.height; i++)
		{
			newImage.data[i * 4 + 0] = src->data[i * 3 + 0];
			newImage.data[i * 4 + 1] = src->data[i * 3 + 1];
			newImage.data[i * 4 + 2] = src->data[i * 3 + 2];
			newImage.data[i * 4 + 3] = 0xff;
		}
		return newImage;
	}

	void ImageUtils::ApplyGammaCorrention(io::ImageData* image)
	{
		uint stride = image->iformat == io::IFI_RGB8 ? 3 : 4;
		for (uint y = 0; y < image->height; y++)
		{
			for (uint x = 0; x < image->width; x++)
			{
				float r, g, b;
				r = float(image->data[(y * image->width + x) * stride + 0]) / 255.0f;
				g = float(image->data[(y * image->width + x) * stride + 1]) / 255.0f;
				b = float(image->data[(y * image->width + x) * stride + 2]) / 255.0f;
				r = powf(r, 2.2f) * 255.0f;
				g = powf(r, 2.2f) * 255.0f;
				b = powf(r, 2.2f) * 255.0f;
				image->data[(y * image->width + x) * stride + 0] = byte(uint(r));
				image->data[(y * image->width + x) * stride + 1] = byte(uint(g));
				image->data[(y * image->width + x) * stride + 2] = byte(uint(b));
			}
		}
	}

	io::ImageData ImageUtils::Clone(const io::ImageData* image)
	{
		ImageData res;
		res.width = image->width;
		res.height = image->height;
		res.bpp = image->bpp;
		res.format = image->format;
		res.iformat = image->iformat;
		res.size = image->size;
		res.data = new byte[image->size];
		res.pitch = image->pitch;
		memcpy(res.data, image->data, image->size);
		return res;
	}

} }