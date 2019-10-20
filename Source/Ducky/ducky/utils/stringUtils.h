#pragma once

namespace ducky { namespace utils {

	class StringUtils
	{
	private:
		static const char* s_Tokens;
		static const char* s_Delimiters;
		static char s_Buffer[50];
	private:
		static char*	Ftoa(char *s, float n);
		static void		Reverse(char str[], int length);
		static char*	Itoa(int num, char* str, int base);
		static char*	Substring(const char* beg, uint len);
		static bool		IsInArray(char c, const char* str);
		static string	AddDecimalsToFloat(string str);
	public:
		static string RemoveComments(const char*);
		static string RemoveComments(const string&);
		static bool StartsWith(const char* src, const char* beg);
		static bool StartsWith(const string& src, const char* beg);
		static bool EndsWith(const string& src, const string& end);
		static vector<string> Tokenize(const char* src);
		static vector<string> Tokenize(const string& src);
		static vector<string> SplitString(const char* src, const char* delimiter);
		static vector<string> SplitString(const string& src, const char* delimiter);
		static int ParseInt(const char*, int base = 10);
		static int ParseInt(const string&, int base = 10);
		static float ParseFloat(const char*);
		static float ParseFloat(const string&);
		static double ParseDouble(const char*);
		static double ParseDouble(const string&);
		static bool IsInt(const char*);
		static bool IsInt(const string&);
		static bool IsFloat(const char*);
		static bool IsFloat(const string&);
		static char* ToString(char* res, float val);
		static char* ToString(char* res, int val, uint base);
		static string ToString(float val);
		static string ToString(int val, uint base);
	};

} }