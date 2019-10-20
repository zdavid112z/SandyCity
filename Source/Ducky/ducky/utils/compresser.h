#pragma once

namespace ducky { namespace utils {

	class Compresser
	{
	public:
		static byte* CompressZ(byte* data, uint32 size, uint32& finalSize);
		static byte* UncompressZ(byte* data, uint32 dataSize, uint32 uncompressedSize);
	};

} }