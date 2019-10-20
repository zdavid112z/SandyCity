#include "ducky_pch.h"
#include "stringUtils.h"

namespace ducky { namespace utils {


	const char* StringUtils::s_Tokens = ",/<>\\][{};():-=+*^#@$%\"\'!";
	const char* StringUtils::s_Delimiters = " \n\t\r";
	char StringUtils::s_Buffer[50];

	char* StringUtils::Ftoa(char *s, float n) {
		if (isnan(n)) {
			strcpy(s, "nan");
		}
		else if (isinf(n)) {
			strcpy(s, "inf");
		}
		else if (n == 0.0) {
			strcpy(s, "0");
		}
		else {
			int digit, m, m1;
			char *c = s;
			int neg = (n < 0);
			if (neg)
				n = -n;
			m = int(log10(n));
			int useExp = (m >= 14 || (neg && m >= 9) || m <= -9);
			if (neg)
				*(c++) = '-';
			if (useExp) {
				if (m < 0)
					m -= 1;
				n = n / pow(10.0f, m);
				m1 = m;
				m = 0;
			}
			if (m < 1.0) {
				m = 0;
			}
			while (n > 0.00001f || m >= 0) {
				float weight = pow(10.0f, m);
				if (weight > 0 && !isinf(weight)) {
					digit = int(floorf(n / weight));
					n -= (digit * weight);
					*(c++) = '0' + digit;
				}
				if (m == 0 && n > 0)
					*(c++) = '.';
				m--;
			}
			if (useExp) {
				int i, j;
				*(c++) = 'e';
				if (m1 > 0) {
					*(c++) = '+';
				}
				else {
					*(c++) = '-';
					m1 = -m1;
				}
				m = 0;
				while (m1 > 0) {
					*(c++) = '0' + m1 % 10;
					m1 /= 10;
					m++;
				}
				c -= m;
				for (i = 0, j = m - 1; i<j; i++, j--) {
					c[i] ^= c[j];
					c[j] ^= c[i];
					c[i] ^= c[j];
				}
				c += m;
			}
			*(c) = '\0';
		}
		return s;
	}

	void StringUtils::Reverse(char str[], int length)
	{
		int start = 0;
		int end = length - 1;
		while (start < end)
		{
			char ax = *(str + start);
			*(str + start) = *(str + end);
			*(str + end) = ax;
			start++;
			end--;
		}
	}

	char* StringUtils::Itoa(int num, char* str, int base)
	{
		int i = 0;
		bool isNegative = false;
		if (num == 0)
		{
			str[i++] = '0';
			str[i] = '\0';
			return str;
		}
		if (num < 0 && base == 10)
		{
			isNegative = true;
			num = -num;
		}

		while (num != 0)
		{
			int rem = num % base;
			str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
			num = num / base;
		}

		if (isNegative)
			str[i++] = '-';

		str[i] = '\0';

		Reverse(str, i);

		return str;
	}

	string StringUtils::AddDecimalsToFloat(string str)
	{
		if (IsInArray('.', str.c_str()))
		{
			uint nd = 0;
			for (uint i = str.size() - 1; str[i] != '.'; i--)
				nd++;
			for (int i = 0; i < 6 - nd; i++)
				str += "0";
		}
		else str += ".000000";
		return str;
	}

	
	char* StringUtils::Substring(const char* beg, uint len)
	{
		char* res = new char[len + 1];
		memcpy(res, beg, len);
		res[len] = NULL;
		return res;
	}

	bool StringUtils::IsInArray(char c, const char* str)
	{
		for (uint i = 0; str[i] != NULL; i++)
			if (c == str[i])
				return true;
		return false;
	}

	char* StringUtils::ToString(char* s, float n)
	{
		return Ftoa(s, n);
	}

	char* StringUtils::ToString(char* s, int n, uint base)
	{
		return Itoa(n, s, base);
	}

	string StringUtils::ToString(float val)
	{
		ToString(s_Buffer, val);
		return AddDecimalsToFloat(string(s_Buffer));
	}

	string StringUtils::ToString(int val, uint base)
	{
		ToString(s_Buffer, val, base);
		return string(s_Buffer);
	}

	vector<string> StringUtils::SplitString(const string& src, const char* delimiter)
	{
		return SplitString(src.c_str(), delimiter);
	}

	vector<string> StringUtils::SplitString(const char* src, const char* delimiter)
	{
		vector<string> res;
		int last;
		int srcSize = strlen(src);
		for (last = 0; IsInArray(src[last], delimiter); last++);
		last--;
		for (int i = last + 1; i < srcSize; i++)
		{
			if (IsInArray(src[i], delimiter))
			{
				if (last + 1 != i)
				{
					res.push_back(string(src + last + 1, i - last - 1));
				}
				last = i;
			}
		}
		if(last + 1 != srcSize)
			res.push_back(string(src + last + 1, srcSize - last - 1));
		return res;
	}

	vector<string> StringUtils::Tokenize(const char* src)
	{
		return Tokenize(string(src));
	}

	vector<string> StringUtils::Tokenize(const string& src)
	{
		vector<string> res;
		unsigned int last = 0;
		bool isIn = false;
		for (last = 0; IsInArray(src[last], s_Delimiters); last++);
		for (uint i = last; i < src.size(); i++)
		{
			if (IsInArray(src[i], s_Tokens))
			{
				if (!isIn)
				{
					res.push_back(src.substr(last, i - last));
					last = i + 1;
				}
				else
				{
					last = i + 1;
				}
				res.push_back(src.substr(i, 1));
				isIn = true;
			}
			else if (IsInArray(src[i], s_Delimiters))
			{
				if (isIn)
				{
					last = i + 1;
				}
				else
				{
					res.push_back(src.substr(last, i - last));
					last = i + 1;
				}
				isIn = true;
			}
			else if (src[i] == '.')
			{
				if (!isdigit(src[last]))
				{
					if (!isIn)
					{
						res.push_back(src.substr(last, i - last));
						last = i + 1;
					}
					else
					{
						last = i + 1;
					}
					res.push_back(src.substr(i, 1));
					isIn = true;
				}
			}
			else
			{
				isIn = false;
			}
		}
		return res;
	}

	bool StringUtils::StartsWith(const char* src, const char* beg)
	{
		for (; *beg != NULL; beg++, src++)
			if (*src != *beg)
				return false;
		return true;
	}

	bool StringUtils::StartsWith(const string& src, const char* beg)
	{
		return StartsWith(src.c_str(), beg);
	}

	bool StringUtils::EndsWith(const string& src, const string& end)
	{
		for (uint i = 0; i < end.size(); i++)
			if (src[src.size() - end.size() + i] != end[i])
				return false;
		return true;
	}

	int StringUtils::ParseInt(const char* str, int base)
	{
		int nr = 0;
		int isNegative = 0;
		if (*str == '-')
		{
			isNegative = -1;
			str++;
		}
		else isNegative = 1;
		while (*str != NULL)
		{
			if (!isdigit(*str))
			{
				//Debug::Printer << WARN << "Trying to parse to integer the string " << org << " and found a non-digit character" << ENDL;
				return nr * isNegative;
			}
			nr = nr * base + *str - '0';
			str++;
		}
		return nr * isNegative;
	}

	int StringUtils::ParseInt(const string& str, int base)
	{
		return ParseInt(str.c_str());
	}

	float StringUtils::ParseFloat(const char* str)
	{
		const char* org = str;
		float nr = 0;
		float exp = 10.0f;
		int isNegative = 1;
		if (*str == '-')
		{
			isNegative = -1;
			str++;
		}
		else isNegative = 1;
		while (isdigit(*str))
		{
			nr = nr * 10 + *str - '0';
			str++;
		}
		if (*str == NULL || *str == 'F' || *str == 'f' || *str == 'd' || *str == 'D')
			return nr * isNegative;
		if (*str == '.')
		{
			str++;
			while (isdigit(*str))
			{
				nr += float((*str - '0') / exp);
				exp *= 10.0f;
				str++;
			}
		}
		if (*str == 'e' || *str == 'E')
		{
			str++;
			float e = 0;
			int isneg = 0;
			if (*str == '-')
			{
				isneg = -1;
				str++;
			}
			else isneg = 1;
			while (isdigit(*str))
			{
				e = e * 10.0f + float(*str - '0');
				str++;
			}
			nr *= powf(10.0f, e);
		}
		if (*str == NULL || *str == 'F' || *str == 'f' || *str == 'd' || *str == 'D')
			return nr * isNegative;
		return nr * isNegative;
	}

	float StringUtils::ParseFloat(const string& str)
	{
		return ParseFloat(str.c_str());
	}

	double StringUtils::ParseDouble(const char* str)
	{
		const char* org = str;
		double nr = 0;
		double exp = 10.0f;
		int isNegative = 1;
		if (*str == '-')
		{
			isNegative = -1;
			str++;
		}
		else isNegative = 1;
		while (isdigit(*str))
		{
			nr = nr * 10 + *str - '0';
			str++;
		}
		if (*str == NULL || *str == 'F' || *str == 'f' || *str == 'd' || *str == 'D')
			return nr * isNegative;
		if (*str == '.')
		{
			str++;
			while (isdigit(*str))
			{
				nr += float((*str - '0') / exp);
				exp *= 10.0f;
				str++;
			}
		}
		if (*str == 'e' || *str == 'E')
		{
			str++;
			double e = 0;
			int isneg = 0;
			if (*str == '-')
			{
				isneg = -1;
				str++;
			}
			else isneg = 1;
			while (isdigit(*str))
			{
				e = e * 10.0 + double(*str - '0');
				str++;
			}
			nr *= pow(10.0f, e);
		}
		if (*str == NULL || *str == 'F' || *str == 'f' || *str == 'd' || *str == 'D')
			return nr * isNegative;
		return nr * isNegative;
	}

	double StringUtils::ParseDouble(const string& str)
	{
		return ParseDouble(str.c_str());
	}

	bool StringUtils::IsInt(const char* str)
	{
		if (*str == '-')
			str++;
		while (*str != NULL)
		{
			if (!isdigit(*str))
				return false;
			str++;
		}
		return true;
	}

	bool StringUtils::IsInt(const string& str)
	{
		return IsInt(str.c_str());
	}

	bool StringUtils::IsFloat(const char* str)
	{
		if (*str == '-')
			str++;
		while (*str != NULL && *str != 'e' && *str != 'E' && *str != '.' && *str != 'f' && *str != 'F' && *str != 'd' && *str != 'D')
		{
			if (!isdigit(*str))
				return false;
			str++;
		}
		if (*str == '.')
		{
			str++;
			while (*str != NULL && *str != 'e' && *str != 'E' && *str != 'f' && *str != 'F' && *str != 'd' && *str != 'D')
			{
				if (!isdigit(*str))
					return false;
				str++;
			}
		}
		if (*str == 'e' || *str == 'E')
		{
			str++;
			if (*str == '-')
				str++;
			while (*str != NULL && *str != 'f' && *str != 'F' && *str != 'd' && *str != 'D')
			{
				if (!isdigit(*str))
					return false;
				str++;
			}
		}
		if (*str == 'f' || *str == 'F' || *str == 'd' || *str == 'D')
			str++;
		if (*str == NULL)
			return true;
		return false;
	}

	bool StringUtils::IsFloat(const string& str)
	{
		return IsFloat(str.c_str());
	}

	string StringUtils::RemoveComments(const char* src)
	{
		return RemoveComments(string(src));
	}

	string StringUtils::RemoveComments(const string& isrc)
	{
		string src = isrc;
		for (uint i = 0; i < src.size() - 1; i++)
		{
			if (src[i] == '/')
			{
				if (src[i + 1] == '/')
					// Line comment
				{
					uint ipos = i;
					for (; src[i] != '\n'; i++);
					i++;
					src.erase(src.begin() + ipos, src.begin() + i);
					i = ipos - 1;
				}
				else if (src[i + 1] == '*')
					// Block comment
				{
					uint ipos = i;
					for (; !StartsWith(&src[i], "*/"); i++);
					i += 2;
					src.erase(src.begin() + ipos, src.begin() + i);
					i = ipos - 1;
				}
			}
		}
		return src;
	}

} }