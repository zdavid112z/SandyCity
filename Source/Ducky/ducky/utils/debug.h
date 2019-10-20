#pragma once

#include "../io/basicio.h"

namespace ducky { namespace utils {

	enum DebugTag
	{
		ENDL, INFO, WARN, ERR, FATAL, VERB, TIME, LOG, TLOG
	};

	class DebugPrinter : public io::OStream
	{
	public:
		DebugPrinter();
		~DebugPrinter();
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

		DebugPrinter& operator<<(const double d) { WriteDouble(d); return *this; }
		DebugPrinter& operator<<(const float d) { WriteFloat(d); return *this; }
		DebugPrinter& operator<<(const int64& d) { WriteInt64(d); return *this; }
		DebugPrinter& operator<<(const uint64& d) { WriteUInt64(d); return *this; }
		DebugPrinter& operator<<(const int& d) { WriteInt(d); return *this; }
		DebugPrinter& operator<<(const uint& d) { WriteUInt(d); return *this; }
		DebugPrinter& operator<<(const short& d) { WriteShort(d); return *this; }
		DebugPrinter& operator<<(const ushort& d) { WriteUShort(d); return *this; }
		DebugPrinter& operator<<(const char& d) { WriteChar(d); return *this; }
		DebugPrinter& operator<<(const byte& d) { WriteByte(d); return *this; }
		DebugPrinter& operator<<(const char* d) { if(d) WriteString(string(d)); return *this; }
		DebugPrinter& operator<<(const string& s) { WriteString(s.c_str()); return *this; }

		DebugPrinter& operator<<(DebugTag t) { WriteTag(t); return *this; }
		void WriteTag(DebugTag);
	private:
		void* m_Handle;
		wchar_t* m_WTemp;
		char* m_Temp;
	};

	class Debug
	{
	public:
		static DebugPrinter Printer;
		static bool Assert(bool exp, const char* message = 0);
	}; 

} }

using namespace ducky::utils;