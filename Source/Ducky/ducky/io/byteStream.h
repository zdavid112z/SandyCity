#pragma once

#include "basicio.h"

namespace ducky { namespace io {

	class BIStream : public IStream
	{
	public:
		BIStream(const byte*, uint size);
		virtual ~BIStream();
	
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

		bool IsOpen() override;
		// Deletes the data
		void Close() override;
		void Seek(uint64 p) override;
		uint64 GetSize() const override;
		uint64 Tell() const override;
	protected:
		const byte* m_Data;
		uint64 m_Index, m_Size;
	};

	class BOStream : public OStream
	{
	public:
		BOStream(uint64 initialSize);
		virtual  ~BOStream();

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

		byte* GetData();
		bool IsOpen() override;
		void Close() override;
		void Seek(uint64) override;
		uint64 GetSize() const override;
		uint64 Tell() const override;
		void CheckAllocated(uint64 size);
	protected:
		vector<byte> m_Data;
		uint64 m_Index;
	};

} }