#pragma once

#include "basicio.h"
#include "serializable.h"

namespace ducky { namespace io {

	enum ImageFormat
	{
		IF_RGB,
		IF_RGBA,
		IF_BGR,
		IF_BGRA,
		IF_RED,
		IF_RG,
		IF_DEPTH,
		IF_DEPTH_STENCIL,
	};

	enum ImageFormatInternal
	{
		IFI_RGB8,
		IFI_RGBA8,
		IFI_RGB16F,
		IFI_RGBA16F,
		IFI_RGB32F,
		IFI_RGBA32F,

		IFI_RED8,
		IFI_RED16F,
		IFI_RED32F,
		  
		IFI_RG8,
		IFI_RG16F,

		IFI_DEPTH16,

		IFI_DEPTH24_STENCIL8,
		IFI_DEPTH32F_STENCIL8,

		IFI_INT8,
		IFI_UINT8,
		IFI_INT16,
		IFI_INT32,
		IFI_UINT16,
		IFI_UINT32

	};

	class ImageData : public Serializable
	{
	public:
		uint width;
		uint height;
		uint size;
		uint bpp;
		uint pitch;
		ImageFormat format;
		ImageFormatInternal iformat;
		byte* data = nullptr;

		void ReadFromFile(const char*);
		void WriteToFile(const char*);
		void Read(IStream*) override;
		void ReadJustHeader(IStream*);
		void Write(OStream*) override;
		void Delete() { if (data) delete[] data; }

		static uint GetImageFormatInternalBPP(ImageFormatInternal);
		static uint GetAPIImageFormat(ImageFormat);
		static uint GetAPIImageFormatInternal(ImageFormatInternal);
	};
} }