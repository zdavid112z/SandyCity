#pragma once

#include "basicio.h"
#include <cstdio>

#ifdef DUCKY_WEB
#define _fseeki64 fseek
#define _ftelli64 ftell
#endif

namespace ducky { namespace io {

	/////////////////////////////////////////////////////////////
	//
	//	Custom objects convention:
	//
	//	4 characters:
	//		- The first one must be 'D'
	//		- The second and third indicate the custom object
	//			e.g. ImageData: IM, ModelData: MD etc
	//			Must be unique
	//		- The fourth one must be 'B' or 'E', indicating
	//		  the beginning, respectively the end of the file
	//
	/////////////////////////////////////////////////////////////

	class FIStream : public IStream
	{
	public:
		FIStream(const char* path);
		~FIStream();
		bool IsOpen() override;
		void Close() override;
		string GetPath() const;
		void Seek(uint64) override;
		uint64 Tell() const override;
		uint64 GetSize() const override;
		void ReadDouble(double&) override;
		void ReadFloat(float&) override;
		void ReadInt(int&) override;
		void ReadUInt(uint&) override;
		void ReadInt64(int64&) override;
		void ReadUInt64(uint64&) override;
		void ReadShort(short&) override;
		void ReadUShort(ushort&) override;
		void ReadChar(char&) override;
		void ReadByte(byte&) override;
		void ReadBytes(byte*, uint) override;
		void ReadStringRaw(char*, uint) override;
		void ReadString(string&) override;
	private:
		string m_Path;
		FILE* m_File;
	};

	class FOStream : public OStream
	{
	public:
		FOStream(const char* path);
		~FOStream();
		bool IsOpen() override;
		void Close() override;
		string GetPath() const;
		void Seek(uint64) override;
		uint64 Tell() const override;
		uint64 GetSize() const override;
		void WriteDouble(const double&) override;
		void WriteFloat(const float&) override;
		void WriteInt(const int&) override;
		void WriteUInt(const uint&) override;
		void WriteInt64(const int64&) override;
		void WriteUInt64(const uint64&) override;
		void WriteShort(const short&) override;
		void WriteUShort(const ushort&) override;
		void WriteChar(const char&) override;
		void WriteByte(const byte&) override;
		void WriteBytes(const byte*, uint) override;
		void WriteStringRaw(const char*, uint) override;
		void WriteString(const string&) override;
	private:
		string m_Path;
		FILE* m_File;
	};

	class FileUtils
	{
	public:
		static string ProcessPath(const string&);
		static string ReadTextFile(const char* loc);
		static byte* ReadBinaryFile(const char* loc, uint& size);
		static void WriteTextFile(const string& data, const char* loc);
		static void WriteBinaryFile(const byte* data, uint dataSize, const char* loc);
		static void WriteEmbeddedFile(const byte* data, uint size, const char* loc);
	};


} }