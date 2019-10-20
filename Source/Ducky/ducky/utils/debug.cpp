#include "ducky_pch.h"
#include "debug.h"
#ifdef DUCKY_WINDOWS
#include <Windows.h>
#endif
#include "stringUtils.h"
#include <cassert>

namespace ducky { namespace utils {

	DebugPrinter Debug::Printer;

	bool Debug::Assert(bool exp, const char* message)
	{
#ifndef DUCKY_DISABLE_DEBUG
		if (!exp)
		{
#ifndef DUCKY_WINDOWS
			Debug::Printer << FATAL << message << ENDL;
#else
			//wchar_t temp[150];
			//mbstowcs(temp, message, 49);
			MessageBox(0, message, "Error", MB_OK | MB_ICONERROR);
#endif
		}
		assert(exp);
#endif
		return exp;
	}

	DebugPrinter::DebugPrinter()
	{
#ifndef DUCKY_DISABLE_DEBUG
#ifdef DUCKY_WINDOWS
		m_Handle = GetStdHandle(STD_OUTPUT_HANDLE);
		m_Temp = new char[300];
		m_WTemp = new wchar_t[150];
#endif
#endif
	}

	DebugPrinter::~DebugPrinter()
	{
#ifndef DUCKY_DISABLE_DEBUG
		WriteTag(VERB);
#ifdef DUCKY_WINDOWS
		delete m_Temp;
		delete m_WTemp;
#endif
#endif
	}

	void DebugPrinter::WriteDouble(const double& v)
	{
#ifndef DUCKY_DISABLE_DEBUG
#ifdef DUCKY_USE_CONSOLE
		printf("%f", v);
#else
		StringUtils::ToString(m_Temp, v);
		//mbstowcs(m_WTemp, m_Temp, 149);
		OutputDebugString(m_Temp);
#endif		
#endif
		
	}

	void DebugPrinter::WriteFloat(const float& v)
	{
#ifndef DUCKY_DISABLE_DEBUG
#ifdef DUCKY_USE_CONSOLE
		printf("%f", v);
#else
		StringUtils::ToString(m_Temp, v);
		//mbstowcs(m_WTemp, m_Temp, 149);
		OutputDebugString(m_Temp);
#endif	
#endif
		
	}

	void DebugPrinter::WriteInt(const int& v)
	{
#ifndef DUCKY_DISABLE_DEBUG
#ifdef DUCKY_USE_CONSOLE
		printf("%i", v);
#else
		StringUtils::ToString(m_Temp, v);
		//mbstowcs(m_WTemp, m_Temp, 149);
		OutputDebugString(m_Temp);
#endif	
#endif
		
	}

	void DebugPrinter::WriteUInt(const uint& v)
	{
#ifndef DUCKY_DISABLE_DEBUG
#ifdef DUCKY_USE_CONSOLE
		printf("%u", v);
#else
		StringUtils::ToString(m_Temp, v);
		//mbstowcs(m_WTemp, m_Temp, 149);
		OutputDebugString(m_Temp);
#endif	
#endif
		
	}

	void DebugPrinter::WriteInt64(const int64& v)
	{
#ifndef DUCKY_DISABLE_DEBUG
#ifdef DUCKY_USE_CONSOLE
		printf("%lli", v);
#else
		StringUtils::ToString(m_Temp, v);
		//mbstowcs(m_WTemp, m_Temp, 149);
		OutputDebugString(m_Temp);
#endif	
#endif
		
	}

	void DebugPrinter::WriteUInt64(const uint64& v)
	{
#ifndef DUCKY_DISABLE_DEBUG
#ifdef DUCKY_USE_CONSOLE
		printf("%llu", v);
#else
		StringUtils::ToString(m_Temp, v);
		//mbstowcs(m_WTemp, m_Temp, 149);
		OutputDebugString(m_Temp);
#endif	
#endif
		
	}

	void DebugPrinter::WriteShort(const short& v)
	{
#ifndef DUCKY_DISABLE_DEBUG
#ifdef DUCKY_USE_CONSOLE
		printf("%i", v);
#else
		StringUtils::ToString(m_Temp, v);
		//mbstowcs(m_WTemp, m_Temp, 149);
		OutputDebugString(m_Temp);
#endif	
#endif
		
	}

	void DebugPrinter::WriteUShort(const ushort& v)
	{
#ifndef DUCKY_DISABLE_DEBUG
#ifdef DUCKY_USE_CONSOLE
		printf("%i", v);
#else
		StringUtils::ToString(m_Temp, v);
		//mbstowcs(m_WTemp, m_Temp, 149);
		OutputDebugString(m_Temp);
#endif	
#endif
		
	}

	void DebugPrinter::WriteChar(const char& v)
	{
#ifndef DUCKY_DISABLE_DEBUG
#ifdef DUCKY_USE_CONSOLE
		printf("%c", v);
#else
		m_Temp[0] = v;
		m_Temp[1] = NULL;
		//mbstowcs(m_WTemp, m_Temp, 149);
		OutputDebugString(m_Temp);
#endif	
#endif
		
	}

	void DebugPrinter::WriteByte(const byte& v)
	{
#ifndef DUCKY_DISABLE_DEBUG
#ifdef DUCKY_USE_CONSOLE
		printf("%i", v);
#else
		StringUtils::ToString(m_Temp, (int)v);
		//mbstowcs(m_WTemp, m_Temp, 149);
		OutputDebugString(m_Temp);
#endif	
#endif
		
	}

	void DebugPrinter::WriteBytes(const byte* v, uint s)
	{
#ifndef DUCKY_DISABLE_DEBUG
#ifdef DUCKY_USE_CONSOLE
		for (uint i = 0; i < s; i++)
			printf("%i", v[i]);
#else
		for (uint i = 0; i < s; i++)
		{
			StringUtils::ToString(m_Temp, v[i]);
			//mbstowcs(m_WTemp, m_Temp, 149);
			OutputDebugString(m_Temp);
		}
#endif	
#endif
		
	}

	void DebugPrinter::WriteStringRaw(const char* v, uint s)
	{
#ifndef DUCKY_DISABLE_DEBUG
#ifdef DUCKY_USE_CONSOLE
		printf("%s", v);
		//for (uint i = 0; i < s; i++)
		//	printf("%c", v[i]);
#else
		//mbstowcs(m_WTemp, v, 149);
		OutputDebugString(m_Temp);
#endif
#endif
		
	}

	void DebugPrinter::WriteString(const string& s)
	{
#ifndef DUCKY_DISABLE_DEBUG
#ifdef DUCKY_USE_CONSOLE
		printf("%s", s.c_str());
		//for (uint i = 0; i < s; i++)
		//	printf("%c", v[i]);
#else
		//mbstowcs(m_WTemp, v, 149);
		OutputDebugString(m_Temp);
#endif
#endif
		
	}

	void DebugPrinter::WriteTag(DebugTag t)
	{
#ifndef DUCKY_DISABLE_DEBUG
		string s;
		switch (t)
		{
		case ENDL:
			WriteChar('\n');
			break;
#ifdef DUCKY_WINDOWS
		case VERB:
			SetConsoleTextAttribute(m_Handle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			break;
		case INFO:
			SetConsoleTextAttribute(m_Handle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case WARN:
			SetConsoleTextAttribute(m_Handle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case ERR:
			SetConsoleTextAttribute(m_Handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case FATAL:
			SetConsoleTextAttribute(m_Handle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED);
			break;
		case LOG:
			WriteString("DUCKY:   ");
			break;
		case TLOG:
			WriteString("DUCKY ");
			WriteTag(TIME);
			WriteString(" : ");
			break;
		case TIME:
			SYSTEMTIME st;
			GetLocalTime(&st);
#ifdef DUCKY_USE_CONSOLE
			printf("[%i:%s%i:%s%i.%s%s%i]", st.wHour, st.wMinute < 10 ? "0" : "", st.wMinute, st.wSecond < 10 ? "0" : "", st.wSecond, st.wMilliseconds < 100 ? "0" : "", st.wMilliseconds < 10 ? "0" : "", st.wMilliseconds);
#else
			s += "[";
			s += st.wHour;
			s += ":";
			s += st.wMinute < 10 ? "0" : "";
			s += st.wMinute;
			s += ":";
			s += st.wSecond < 10 ? "0" : "";
			s += st.wSecond;
			s += ".";
			s += st.wMilliseconds < 100 ? "0" : "";
			s += st.wMilliseconds < 10 ? "0" : "";
			s += st.wMilliseconds;
			//mbstowcs(m_WTemp, s.c_str(), 149);
			OutputDebugString(m_Temp);
#endif
			break;
#endif
		default:
			break;
		};
#endif
		
	}

} }