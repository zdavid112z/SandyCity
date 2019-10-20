#include "ducky_pch.h"
#include "../utils/debug.h"
#include "../utils/stringUtils.h"
#include "fileio.h"

using namespace ducky::utils;

namespace ducky { namespace io {

	FIStream::FIStream(const char* path)
	{
		m_Path = FileUtils::ProcessPath(path);
		m_File = fopen(m_Path.c_str(), "rb");
	}

	FIStream::~FIStream()
	{
		Close();
	}

	bool FIStream::IsOpen() 
	{
		return m_File != NULL;
	}

	void FIStream::Seek(uint64 pos)
	{
		_fseeki64(m_File, pos, SEEK_SET);
	}

	uint64 FIStream::Tell() const
	{
		return (uint64)_ftelli64(m_File);
	}

	uint64 FIStream::GetSize() const
	{
		uint64 pos = _ftelli64(m_File);
		_fseeki64(m_File, 0, SEEK_END);
		uint64 res = _ftelli64(m_File);
		return res;
		_fseeki64(m_File, pos, SEEK_SET);
	}

	void FIStream::Close()
	{
		if (m_File == 0)
			return;
		fclose(m_File);
		m_File = 0;
	}

	string FIStream::GetPath() const
	{
		return m_Path;
	}

	void FIStream::ReadDouble(double& v)
	{
		fread(&v, 1, sizeof(double), m_File);
		
	}

	void FIStream::ReadFloat(float& v)
	{
		fread(&v, 1, sizeof(float), m_File);
		
	}

	void FIStream::ReadInt(int& v)
	{
		fread(&v, 1, sizeof(int), m_File);
		
	}

	void FIStream::ReadUInt(uint& v)
	{
		fread(&v, 1, sizeof(uint), m_File);
		
	}

	void FIStream::ReadInt64(int64& v)
	{
		fread(&v, 1, sizeof(int64), m_File);
		
	}

	void FIStream::ReadUInt64(uint64& v)
	{
		fread(&v, 1, sizeof(uint64), m_File);
		
	}

	void FIStream::ReadShort(short& v)
	{
		fread(&v, 1, sizeof(short), m_File);
		
	}

	void FIStream::ReadUShort(ushort& v)
	{
		fread(&v, 1, sizeof(ushort), m_File);
		
	}

	void FIStream::ReadChar(char& v)
	{
		fread(&v, 1, sizeof(char), m_File);
		
	}

	void FIStream::ReadByte(byte& v)
	{
		fread(&v, 1, sizeof(byte), m_File);
		
	}

	void FIStream::ReadBytes(byte* v, uint s)
	{
		fread(v, 1, s, m_File);
		
	}

	void FIStream::ReadStringRaw(char* v, uint s)
	{
		fread(v, 1, s, m_File);
		
	}

	void FIStream::ReadString(string& s)
	{
		uint16 size;
		ReadUShort(size);
		char* data = new char[size + 1];
		ReadStringRaw(data, size);
		data[size] = NULL;
		s = string(data);
		delete data;
		
	}






	FOStream::FOStream(const char* path)
	{
		m_Path = FileUtils::ProcessPath(path);
		m_File = fopen(m_Path.c_str(), "wb");
	}

	FOStream::~FOStream()
	{
		Close();
	}

	bool FOStream::IsOpen()
	{
		return m_File != NULL;
	}

	void FOStream::Seek(uint64 pos)
	{
		_fseeki64(m_File, pos, SEEK_SET);
	}
	
	uint64 FOStream::Tell() const
	{
		return (uint64)_ftelli64(m_File);
	}

	uint64 FOStream::GetSize() const
	{
		uint64 pos = _ftelli64(m_File);
		_fseeki64(m_File, 0, SEEK_END);
		uint64 res = _ftelli64(m_File);
		_fseeki64(m_File, pos, SEEK_SET);
		return res;
	}

	void FOStream::Close()
	{
		if (m_File == 0)
			return;
		fclose(m_File);
		m_File = 0;
	}

	string FOStream::GetPath() const
	{
		return m_Path;
	}

	void FOStream::WriteDouble(const double& v)
	{
		fwrite(&v, 1, sizeof(double), m_File);
		
	}

	void FOStream::WriteFloat(const float& v)
	{
		fwrite(&v, 1, sizeof(float), m_File);
		
	}

	void FOStream::WriteInt(const int& v)
	{
		fwrite(&v, 1, sizeof(int), m_File);
		
	}

	void FOStream::WriteUInt(const uint& v)  
	{
		fwrite(&v, 1, sizeof(uint), m_File);
		
	}

	void FOStream::WriteInt64(const int64& v)
	{
		fwrite(&v, 1, sizeof(int64), m_File);
		
	}

	void FOStream::WriteUInt64(const uint64& v)
	{
		fwrite(&v, 1, sizeof(uint64), m_File);
		
	}

	void FOStream::WriteShort(const short& v)  
	{
		fwrite(&v, 1, sizeof(short), m_File);
		
	}

	void FOStream::WriteUShort(const ushort& v)  
	{
		fwrite(&v, 1, sizeof(ushort), m_File);
		
	}

	void FOStream::WriteChar(const char& v)  
	{
		fwrite(&v, 1, sizeof(char), m_File);
		
	}

	void FOStream::WriteByte(const byte& v)  
	{
		fwrite(&v, 1, sizeof(byte), m_File);
		
	}

	void FOStream::WriteBytes(const byte* v, uint s)  
	{
		fwrite(v, 1, s, m_File);
		
	}

	void FOStream::WriteStringRaw(const char* v, uint s)  
	{
		fwrite(v, 1, s, m_File);
		
	}

	void FOStream::WriteString(const string& s)
	{
		WriteUShort(s.size());
		WriteStringRaw(s.c_str(), s.size());
		
	}

	string FileUtils::ReadTextFile(const char* cloc)
	{
		string loc = ProcessPath(cloc);
		FILE* file = fopen(loc.c_str(), "rt");
		if (file == NULL)
		{
			Debug::Printer << FATAL << "Could not locate file " << loc << "!\n";
			Debug::Assert(false);
		}
		string text;
		char buffer[1024];
		int r;
		while ((r = fread(buffer, 1, 1024, file)) > 0)
		{
			text.append(buffer, buffer + r);
		}
		fclose(file);
		return text;
	}

	void FileUtils::WriteTextFile(const string& text, const char* loc)
	{
		FILE* file = fopen(ProcessPath(loc).c_str(), "wt");
		fwrite(text.c_str(), 1, text.size(), file);
		fclose(file);
	}

	byte* FileUtils::ReadBinaryFile(const char* loc, uint& size)
	{
		string sloc = ProcessPath(loc);
		FILE* file = fopen(sloc.c_str(), "rb");
		if (file == NULL)
		{
			Debug::Printer << FATAL << "Could not locate binary file " << sloc << "!\n";
			Debug::Assert(false);
		}
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		byte* data = new byte[size];
		fseek(file, 0, SEEK_SET);
		fread(data, 1, size, file);
		fclose(file);
		return data;
	}

	void FileUtils::WriteBinaryFile(const byte* data, uint dataSize, const char* loc)
	{
		string path = ProcessPath(loc);
		FILE* file = fopen(path.c_str(), "wb");
		fwrite(data, 1, dataSize, file);
		fclose(file);
	}

	void FileUtils::WriteEmbeddedFile(const byte* data, uint size, const char* loc)
	{
		string path = ProcessPath(loc);
		FILE* file = fopen(path.c_str(), "wt");
		const ushort* sdata = (const ushort*)data;
		size = (size + 1) / 2;
		fprintf(file, "%i", sdata[0]);
		for (uint i = 1; i < size; i++)
		{
			fprintf(file, ",%i", sdata[i]);
		}
		fclose(file);
	}

	string FileUtils::ProcessPath(const string& org)
	{
		string s = org;
		return s;
	}

} }