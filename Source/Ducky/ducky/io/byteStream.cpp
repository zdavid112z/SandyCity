#include "ducky_pch.h"
#include "byteStream.h"
#include "../utils/debug.h"

using ducky::utils::Debug;

namespace ducky { namespace io {

	BIStream::BIStream(const byte* data, uint size)
	{
		m_Data = data;
		m_Size = size;
		m_Index = 0;
	}

	BIStream::~BIStream()
	{

	}

	bool BIStream::IsOpen()
	{
		return m_Data != nullptr;
	}

	void BIStream::Close()
	{
		if(m_Data != nullptr)
			delete[] m_Data;
		m_Data = nullptr;
	}

	void BIStream::ReadDouble(double& v)
	{
		v = *(double*)(m_Data + m_Index);
		m_Index += sizeof(double);
		
	}

	void BIStream::ReadFloat(float& v)
	{
		v = *(float*)(m_Data + m_Index);
		m_Index += sizeof(float);
		
	}

	void BIStream::ReadInt(int& v)
	{
		v = *(int*)(m_Data + m_Index);
		m_Index += sizeof(int);
		
	}

	void BIStream::ReadUInt(uint& v)
	{
		v = *(uint*)(m_Data + m_Index);
		m_Index += sizeof(uint);
		
	}

	void BIStream::ReadInt64(int64& v)
	{
		v = *(int64*)(m_Data + m_Index);
		m_Index += sizeof(int64);
		
	}

	void BIStream::ReadUInt64(uint64& v)
	{
		v = *(uint64*)(m_Data + m_Index);
		m_Index += sizeof(uint64);
		
	}

	void BIStream::ReadShort(short& v)
	{
		v = *(short*)(m_Data + m_Index);
		m_Index += sizeof(short);
		
	}

	void BIStream::ReadUShort(ushort& v)
	{
		v = *(ushort*)(m_Data + m_Index);
		m_Index += sizeof(ushort);
		
	}

	void BIStream::ReadChar(char& v)
	{
		v = *(char*)(m_Data + m_Index);
		m_Index += sizeof(char);
		
	}

	void BIStream::ReadByte(byte& v)
	{
		v = *(byte*)(m_Data + m_Index);
		m_Index += sizeof(byte);
		
	}

	void BIStream::ReadBytes(byte* v, uint vs)
	{
		memcpy(v, m_Data + m_Index, vs);
		m_Index += vs;
		
	}

	void BIStream::ReadStringRaw(char* v, uint vs)
	{
		memcpy(v, m_Data + m_Index, vs);
		m_Index += vs;
		
	}

	void BIStream::ReadString(string& s)
	{
		uint16 size;
		ReadUShort(size);
		s = string((char*)m_Data + m_Index, size);
		m_Index += size;
		
	}

	void BIStream::Seek(uint64 p)
	{
		if (p > m_Size)
			m_Index = m_Size;
		else m_Index = p;
	}

	uint64 BIStream::GetSize() const 
	{
		return m_Size;
	}

	uint64 BIStream::Tell() const
	{
		return m_Index;
	}






	BOStream::BOStream(uint64 initialSize)
	{
		m_Data = vector<byte>(initialSize, 0);
		m_Index = 0;
	}

	BOStream::~BOStream()
	{
		m_Data.clear();
	}

	void BOStream::CheckAllocated(uint64 size)
	{
		m_Data.reserve(size);
	}

	void BOStream::WriteDouble(const double& v)
	{
		uint64 s = sizeof(double);
		CheckAllocated(s);
		memcpy(m_Data.data() + m_Index, &v, s);
		m_Index += s;
		
	}

	void BOStream::WriteFloat(const float& v)
	{
		uint64 s = sizeof(float);
		CheckAllocated(s);
		memcpy(m_Data.data() + m_Index, &v, s);
		m_Index += s;
		
	}

	void BOStream::WriteInt(const int& v)
	{
		uint64 s = sizeof(int);
		CheckAllocated(s);
		memcpy(m_Data.data() + m_Index, &v, s);
		m_Index += s;
		
	}

	void BOStream::WriteUInt(const uint& v)
	{
		uint64 s = sizeof(uint);
		CheckAllocated(s);
		memcpy(m_Data.data() + m_Index, &v, s);
		m_Index += s;
		
	}

	void BOStream::WriteInt64(const int64& v)
	{
		uint64 s = sizeof(int64);
		CheckAllocated(s);
		memcpy(m_Data.data() + m_Index, &v, s);
		m_Index += s;
		
	}

	void BOStream::WriteUInt64(const uint64& v)
	{
		uint64 s = sizeof(uint64);
		CheckAllocated(s);
		memcpy(m_Data.data() + m_Index, &v, s);
		m_Index += s;
		
	}

	void BOStream::WriteShort(const short& v)
	{
		uint64 s = sizeof(short);
		CheckAllocated(s);
		memcpy(m_Data.data() + m_Index, &v, s);
		m_Index += s;
		
	}

	void BOStream::WriteUShort(const ushort& v)
	{
		uint64 s = sizeof(ushort);
		CheckAllocated(s);
		memcpy(m_Data.data() + m_Index, &v, s);
		m_Index += s;
		
	}

	void BOStream::WriteChar(const char& v)
	{
		uint64 s = sizeof(char);
		CheckAllocated(s);
		memcpy(m_Data.data() + m_Index, &v, s);
		m_Index += s;
		
	}

	void BOStream::WriteByte(const byte& v)
	{
		uint64 s = sizeof(byte);
		CheckAllocated(s);
		memcpy(m_Data.data() + m_Index, &v, s);
		m_Index += s;
		
	}

	void BOStream::WriteBytes(const byte* v, uint s)
	{
		CheckAllocated(s);
		memcpy(m_Data.data() + m_Index, v, s);
		m_Index += s;
		
	}

	void BOStream::WriteStringRaw(const char* v, uint s)
	{
		CheckAllocated(s);
		memcpy(m_Data.data() + m_Index, v, s);
		m_Index += s;
		
	}

	void BOStream::WriteString(const string& v)
	{
		WriteUShort(v.size());
		WriteStringRaw(v.c_str(), v.size());
		
	}

	byte* BOStream::GetData()
	{
		return m_Data.data();
	}

	bool BOStream::IsOpen()
	{
		return true;
	}

	void BOStream::Close()
	{
		m_Data.shrink_to_fit();
	}

	void BOStream::Seek(uint64 p)
	{
		m_Index = p;
	}

	uint64 BOStream::GetSize() const
	{
		return m_Data.capacity();
	}

	uint64 BOStream::Tell() const
	{
		return m_Index;
	}


} }