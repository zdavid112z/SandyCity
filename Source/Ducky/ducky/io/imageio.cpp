#include "ducky_pch.h"
#include "imageio.h"

#ifdef DUCKY_DIRECTX
#include <d3d11.h>
#elif defined DUCKY_OPENGL
#include "graphics\opengl.h"
#endif

#include "../utils/debug.h"
#include <freeimage/FreeImage.h>
#include <freeimage/Utilities.h>
#include "utils\compresser.h"
#include "fileio.h"

using namespace ducky::utils;

namespace ducky { namespace io {

	void ImageData::ReadFromFile(const char* cpath)
	{
		if (data)
			delete[] data;
		string path = FileUtils::ProcessPath(cpath);
		FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(path.c_str());
		if (fif == -1)
		{
			Debug::Printer << ERR << "Could not locate image " << path << '\n';
			return;
		}
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(path.c_str());
		if (!FreeImage_FIFSupportsReading(fif))
		{
			Debug::Printer << ERR << "Cannot read image " << path << '\n';
			return;
		}
		FIBITMAP* bitmap = FreeImage_Load(fif, path.c_str());
		width = FreeImage_GetWidth(bitmap);
		height = FreeImage_GetHeight(bitmap);
		bpp = FreeImage_GetBPP(bitmap);
		pitch = FreeImage_GetPitch(bitmap);
		FREE_IMAGE_TYPE type = FreeImage_GetImageType(bitmap);
		switch (type)
		{
		case FIT_BITMAP:
			if (bpp == 24)
			{
				SwapRedBlue32(bitmap);
				format = IF_RGB;
				iformat = IFI_RGB8;
			}
			else if (bpp == 32)
			{
				SwapRedBlue32(bitmap);
				format = IF_RGBA;
				iformat = IFI_RGBA8;
			}
			else if (bpp == 16)
			{
				format = IF_RG;
				iformat = IFI_RG8;
			}
			else
			{
				format = IF_RED;
				iformat = IFI_RED8;
			}
			break;
		case FIT_FLOAT:
			format = IF_RED;
			iformat = IFI_RED16F;
		case FIT_UINT16:
			format = IF_RED;
			iformat = IFI_RED16F;
			break;
		case FIT_UINT32:
			format = IF_RED;
			iformat = IFI_UINT32;
			break;
		case FIT_INT16:
			format = IF_RED;
			iformat = IFI_INT16;
			break;
		case FIT_INT32:
			format = IF_RED;
			iformat = IFI_INT32;
			break;
		}
		size = pitch * height;
		data = new byte[size];
		memcpy(data, FreeImage_GetBits(bitmap), size);
		FreeImage_Unload(bitmap);
	}

	void ImageData::WriteToFile(const char* path)
	{
		//FIBITMAP* b = FreeImage_ConvertFromRawBits(data, width, height, width * bpp / 8, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, 1);
		FIBITMAP* b = FreeImage_ConvertFromRawBitsEx(false, data, FIT_BITMAP, width, height, pitch, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);
		SwapRedBlue32(b);
		BOOL res = FreeImage_Save(FIF_PNG, b, path);
		if (res == 0)
		{
			Debug::Printer << WARN << "Could not output png image to " << path << ENDL;
		}
	}

	void ImageData::Read(IStream* in)
	{
		if (data)
			delete[] data;
		in->ReadUInt(width);
		in->ReadUInt(height);
		in->ReadUInt(size);
		in->ReadUInt(bpp);
		in->ReadUInt(pitch);

		uint ax;
		in->ReadUInt(ax);
		format = (ImageFormat)ax;
		in->ReadUInt(ax);
		iformat = (ImageFormatInternal)ax;

		uint32 compressedSize;
		in->ReadUInt(compressedSize);
		byte* compressed = new byte[compressedSize];
		in->ReadBytes(compressed, compressedSize);
		data = utils::Compresser::UncompressZ(compressed, compressedSize, size);
		delete[] compressed;

		//Debug::Printer << INFO << TLOG << "Loaded image from stream." << ENDL;
		//Debug::Printer << INFO << TLOG << "Compressed size: " << compressedSize << ENDL;
		//Debug::Printer << INFO << TLOG << "Original size:   " << size << ENDL << ENDL;
	}

	void ImageData::ReadJustHeader(IStream* in)
	{
		in->ReadUInt(width);
		in->ReadUInt(height);
		in->ReadUInt(size);
		in->ReadUInt(bpp);
		in->ReadUInt(pitch);

		uint ax;
		in->ReadUInt(ax);
		format = (ImageFormat)ax;
		in->ReadUInt(ax);
		iformat = (ImageFormatInternal)ax;
	}

	void ImageData::Write(OStream* out)
	{
		out->WriteUInt(width);
		out->WriteUInt(height);
		out->WriteUInt(size);
		out->WriteUInt(bpp);
		out->WriteUInt(pitch);
		out->WriteUInt(format);
		out->WriteUInt(iformat);

		uint32 compressedSize = 0;
		byte* compressed;
		compressed = utils::Compresser::CompressZ(data, size, compressedSize);
		out->WriteUInt(compressedSize);
		out->WriteBytes(compressed, compressedSize);
		delete[] compressed;

		//Debug::Printer << INFO << TLOG << "Saved image to stream." << ENDL;
		//Debug::Printer << INFO << TLOG << "Original size:   " << size << ENDL;
		//Debug::Printer << INFO << TLOG << "Compressed size: " << compressedSize << ENDL << ENDL;
	}

	uint ImageData::GetImageFormatInternalBPP(ImageFormatInternal ifi)
	{
		switch (ifi)
		{
		case ducky::io::IFI_RGB8:
			return 24;
			break;
		case ducky::io::IFI_RGBA8:
			return 32;
			break;
		case ducky::io::IFI_RGB16F:
			return 48;
			break;
		case ducky::io::IFI_RGBA16F:
			return 64;
			break;
		case ducky::io::IFI_RGB32F:
			return 96;
			break;
		case ducky::io::IFI_RGBA32F:
			return 128;
			break;
		case ducky::io::IFI_RED8:
			return 8;
			break;
		case ducky::io::IFI_RED16F:
			return 16;
			break;
		case ducky::io::IFI_RED32F:
			return 32;
			break;
		case ducky::io::IFI_RG8:
			return 16;
			break;
		case ducky::io::IFI_RG16F:
			return 32;
			break;
		case ducky::io::IFI_DEPTH16:
			return 16;
			break;
		case ducky::io::IFI_DEPTH24_STENCIL8:
			return 32;
			break;
		case ducky::io::IFI_DEPTH32F_STENCIL8:
			return 40;
			break;
		case ducky::io::IFI_INT16:
			return 16;
			break;
		case ducky::io::IFI_INT32:
			return 32;
			break;
		case ducky::io::IFI_UINT16:
			return 16;
			break;
		case ducky::io::IFI_UINT32:
			return 32;
			break;
		case ducky::io::IFI_INT8:
			return 8;
			break;
		case ducky::io::IFI_UINT8:
			return 8;
			break;
		default:
			return 0;
			break;
		}
		return 0;
	}

#ifdef DUCKY_DIRECTX

	uint ImageData::GetAPIImageFormat(ImageFormat f)
	{
		switch (f)
		{
		case ducky::io::IF_RGB:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		case ducky::io::IF_RGBA:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
#if DUCKY_OPENGL < 33
		case ducky::io::IF_BGR:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		case ducky::io::IF_BGRA:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
#else
		case ducky::io::IF_BGR:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		case ducky::io::IF_BGRA:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
#endif
		case ducky::io::IF_RED:
			return DXGI_FORMAT_R8_UNORM;
			break;
		case ducky::io::IF_RG:
			return DXGI_FORMAT_R8G8_UNORM;
			break;
		case ducky::io::IF_DEPTH:
			return DXGI_FORMAT_D16_UNORM;
			break;
		case ducky::io::IF_DEPTH_STENCIL:
			return DXGI_FORMAT_D24_UNORM_S8_UINT;
			break;
		default:
			return 0;
			break;
		}
		return 0;
	}

	uint ImageData::GetAPIImageFormatInternal(ImageFormatInternal ifi)
	{
		switch (ifi)
		{
		case ducky::io::IFI_RGB8:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		case ducky::io::IFI_RGBA8:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		case ducky::io::IFI_RGB16F:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;
			break;
		case ducky::io::IFI_RGBA16F:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;
			break;
		case ducky::io::IFI_RGB32F:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		case ducky::io::IFI_RGBA32F:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		case ducky::io::IFI_RED8:
			return DXGI_FORMAT_R8_SINT;
			break;
		case ducky::io::IFI_RED16F:
			return DXGI_FORMAT_R16_FLOAT;
			break;
		case ducky::io::IFI_RED32F:
			return DXGI_FORMAT_R32_FLOAT;
			break;
		case ducky::io::IFI_RG8:
			return DXGI_FORMAT_R8G8_UNORM;
			break;
		case ducky::io::IFI_RG16F:
			return DXGI_FORMAT_R16G16_FLOAT;
			break;
		case ducky::io::IFI_DEPTH16:
			return DXGI_FORMAT_D16_UNORM;
			break;
		case ducky::io::IFI_DEPTH32:
			return DXGI_FORMAT_D32_FLOAT;
			break;
		case ducky::io::IFI_DEPTH24_STENCIL8:
			return DXGI_FORMAT_D24_UNORM_S8_UINT;
			break;
		case ducky::io::IFI_DEPTH32F_STENCIL8:
			return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
			break;
		case ducky::io::IFI_INT16:
			return DXGI_FORMAT_R16_SINT;
			break;
		case ducky::io::IFI_INT32:
			return DXGI_FORMAT_R32_SINT;
			break;
		case ducky::io::IFI_UINT16:
			return DXGI_FORMAT_R16_UINT;
			break;
		case ducky::io::IFI_UINT32:
			return DXGI_FORMAT_R32_UINT;
			break;
		case ducky::io::IFI_INT8:
			return DXGI_FORMAT_R8_SINT;
			break;
		case ducky::io::IFI_UINT8:
			return DXGI_FORMAT_R8_UINT;
			break;
		default:
			return 0;
			break;
		}
		return 0;
	}

#elif defined DUCKY_OPENGL

	uint ImageData::GetAPIImageFormat(ImageFormat f)
	{
		switch (f)
		{
		case ducky::io::IF_RGB:
			return GL_RGB;
			break;
		case ducky::io::IF_RGBA:
			return GL_RGBA;
			break;
#ifdef DUCKY_GLES
		case ducky::io::IF_BGR:
			return GL_RGB;
			break;
		case ducky::io::IF_BGRA:
			return GL_RGBA;
			break;
#else
		case ducky::io::IF_BGR:
			return GL_BGR;
			break;
		case ducky::io::IF_BGRA:
			return GL_BGRA;
			break;
#endif
		case ducky::io::IF_RED:
			return GL_RED;
			break;
		case ducky::io::IF_RG:
			return GL_RG;
			break;
		case ducky::io::IF_DEPTH:
			return GL_DEPTH_COMPONENT;
			break;
		case ducky::io::IF_DEPTH_STENCIL:
			return GL_DEPTH_STENCIL;
			break;
		default:
			return 0;
			break;
		}
		return 0;
	}

	uint ImageData::GetAPIImageFormatInternal(ImageFormatInternal ifi)
	{
		switch (ifi)
		{
		case ducky::io::IFI_RGB8:
			return GL_RGB;
			break;
		case ducky::io::IFI_RGBA8:
			return GL_RGBA;
			break;
		case ducky::io::IFI_RGB16F:
			return GL_RGB16F;
			break;
		case ducky::io::IFI_RGBA16F:
			return GL_RGBA16F;
			break;
		case ducky::io::IFI_RGB32F:
			return GL_RGB32F;
			break;
		case ducky::io::IFI_RGBA32F:
			return GL_RGBA32F;
			break;
		case ducky::io::IFI_RED8:
			return GL_R8;
			break;
		case ducky::io::IFI_RED16F:
			return GL_R16F;
			break;
		case ducky::io::IFI_RED32F:
			return GL_R32F;
			break;
		case ducky::io::IFI_RG8:
			return GL_RG8;
			break;
		case ducky::io::IFI_RG16F:
			return GL_RG16F;
			break;
		case ducky::io::IFI_DEPTH16:
			return GL_DEPTH_COMPONENT16;
			break;
		case ducky::io::IFI_DEPTH24_STENCIL8:
			return GL_DEPTH24_STENCIL8;
			break;
		case ducky::io::IFI_DEPTH32F_STENCIL8:
			return GL_DEPTH32F_STENCIL8;
			break;
		case ducky::io::IFI_INT16:
			return GL_R16I;
			break;
		case ducky::io::IFI_INT32:
			return GL_R32I;
			break;
		case ducky::io::IFI_UINT16:
			return GL_R16UI;
			break;
		case ducky::io::IFI_UINT32:
			return GL_R32UI;
			break;
		case ducky::io::IFI_INT8:
			return GL_R8I;
			break;
		case ducky::io::IFI_UINT8:
			return GL_R8UI;
			break;
		default:
			return 0;
			break;
		}
		return 0;
	}

#endif

} }