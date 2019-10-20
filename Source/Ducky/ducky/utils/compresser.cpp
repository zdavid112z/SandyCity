#include "ducky_pch.h"
#include "compresser.h"
#include "debug.h"
#include "zlib\zlib.h"

namespace ducky { namespace utils {

	byte* Compresser::CompressZ(byte* data, uint32 size, uint32& finalSize)
	{
		finalSize = size * 1.1 + 12;
		byte* res = new byte[finalSize];
		int zres = compress(res, (uLongf*)&finalSize, data, size);
		switch (zres)
		{
		case Z_OK:
			return res;
			break;
		case Z_MEM_ERROR:
			Debug::Printer << ERR << "Zlib Compression failed: Out of Memory!" << ENDL;
			break;
		case Z_BUF_ERROR:
			Debug::Printer << ERR << "Zlib Compression failed: Output buffer not big enough! (Should not happen)" << ENDL;
			break;
		}
		return nullptr;
	}

	byte* Compresser::UncompressZ(byte* data, uint32 dataSize, uint32 uncompressedSize)
	{
		byte* res = new byte[uncompressedSize];
		int zres = uncompress(res, (uLongf*)&uncompressedSize, data, dataSize);
		switch (zres)
		{
		case Z_OK:
			return res;
			break;
		case Z_MEM_ERROR:
			Debug::Printer << ERR << "Zlib Compression failed: Out of Memory!" << ENDL;
			break;
		case Z_BUF_ERROR:
			Debug::Printer << ERR << "Zlib Compression failed: Output buffer not big enough! (Should not happen)" << ENDL;
			break;
		}
		return nullptr;
	}

} }