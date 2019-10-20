#pragma once

#include "ducky_pch.h"

namespace ducky { namespace io {

	class IStream
	{
	public:
		virtual void ReadDouble(double&) = 0;
		virtual void ReadFloat(float&) = 0;
		virtual void ReadInt(int&) = 0;
		virtual void ReadUInt(uint&) = 0;
		virtual void ReadInt64(int64&) = 0;
		virtual void ReadUInt64(uint64&) = 0;
		virtual void ReadShort(short&) = 0;
		virtual void ReadUShort(ushort&) = 0;
		virtual void ReadChar(char&) = 0;
		virtual void ReadByte(byte&) = 0;
		virtual void ReadBytes(byte*, uint) = 0;
		virtual void ReadStringRaw(char*, uint) = 0;
		virtual void ReadString(string&) = 0;

		virtual bool IsOpen() { return false; }
		virtual void Close() {}
		virtual void Seek(uint64) {}
		virtual uint64 GetSize() const { return 0; }
		virtual uint64 Tell() const { return 0; }
	};

	class OStream
	{
	public:
		virtual void WriteDouble(const double&) = 0;
		virtual void WriteFloat(const float&) = 0;
		virtual void WriteInt(const int&) = 0;
		virtual void WriteUInt(const uint&) = 0;
		virtual void WriteInt64(const int64&) = 0;
		virtual void WriteUInt64(const uint64&) = 0;
		virtual void WriteShort(const short&) = 0;
		virtual void WriteUShort(const ushort&) = 0;
		virtual void WriteChar(const char&) = 0;
		virtual void WriteByte(const byte&) = 0;
		virtual void WriteBytes(const byte*, uint) = 0;
		virtual void WriteStringRaw(const char*, uint) = 0;
		virtual void WriteString(const string&) = 0;

		virtual bool IsOpen() { return false; }
		virtual void Close() {}
		virtual void Seek(uint64) {}
		virtual uint64 GetSize() const { return 0; }
		virtual uint64 Tell() const { return 0; }
	};

} }