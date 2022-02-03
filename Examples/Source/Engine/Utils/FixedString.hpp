#if !defined(_ELIA2D_FIXED_STRING_H_)
#define _ELIA2D_FIXED_STRING_H_

#pragma once
#include <string.h>
#include <string>
#include <assert.h>
#include <stdarg.h>

#pragma warning(push)
#pragma warning(disable : 4505)

class FixedString256
{
public:
	FixedString256()
	{
		memset(myData, 0, 256);
	}
	FixedString256(const char* const aString)
	{
		memcpy(myData, aString, 256);
	}
	FixedString256(const FixedString256& anotherString)
	{
		memcpy(myData, anotherString, 256);
	}
	FixedString256(FixedString256&& aMoveableString) noexcept
	{
		memcpy(myData, aMoveableString, 256);
	}
	FixedString256(const std::string& aSTLString)
	{
		assert(aSTLString.size() < 256);
		if (aSTLString.size() < 256)
		{
			strcpy_s(myData, aSTLString.data());
		}
		else
		{
			strcpy_s(myData, "errors n stuff");
		}
	}
	static FixedString256 Format(const char* const aFormat, ...)
	{
		FixedString256 str;
		va_list vl;
		va_start(vl, aFormat);
		vsprintf_s(str.myData, aFormat, vl);
		va_end(vl);
		return str;
	}

	operator const char* () const
	{
		return myData;
	}
	operator char* ()
	{
		return myData;
	}
	FixedString256& operator=(const FixedString256& anotherString)
	{
		memcpy(myData, anotherString, 256);

		return *this;
	}
	FixedString256& operator=(FixedString256&& aMoveableString) noexcept
	{
		memcpy(myData, aMoveableString, 256);

		return *this;
	}
	FixedString256& operator=(const char* const aCString)
	{
		strcpy_s(myData, aCString);

		return *this;
	}
	char* Data()
	{
		return myData;
	}

private:
	char myData[256];
};

static bool operator<(const FixedString256& aString1, const FixedString256& aString2)
{
	return strcmp(aString1, aString2) < -0;
}
static bool operator>(const FixedString256& aString1, const FixedString256& aString2)
{
	return strcmp(aString1, aString2) > 0;
}
static bool operator==(const FixedString256& aString1, const FixedString256& aString2)
{
	return strcmp(aString1, aString2) == 0;
}
static FixedString256 operator+(const FixedString256& aString1, const FixedString256& aString2)
{
	FixedString256 str = aString1;
	strcat_s(str, 256 - strlen(aString1), aString2);
	return str;
}

class FixedWString256
{
public:
	FixedWString256()
	{
		wcscpy_s(myData, L"");
	}
	FixedWString256(const wchar_t* const aString)
	{
		wcscpy_s(myData, aString);
	}
	FixedWString256(const FixedWString256& anotherString)
	{
		wcscpy_s(myData, anotherString);
	}
	FixedWString256(FixedWString256&& aMoveableString) noexcept
	{
		wcscpy_s(myData, aMoveableString);
	}
	FixedWString256(const std::wstring& aSTLString)
	{
		assert(aSTLString.size() < 256);
		if (aSTLString.size() < 256)
		{
			wcscpy_s(myData, aSTLString.data());
		}
		else
		{
			wcscpy_s(myData, L"errors n stuff");
		}
	}
	static FixedWString256 Format(const wchar_t* const aFormat, ...)
	{
		FixedWString256 str;
		va_list vl;
		va_start(vl, aFormat);
		vswprintf_s(str.myData, aFormat, vl);
		va_end(vl);
		return str;
	}

#pragma warning(push)
#pragma warning(disable : 4114)
	operator const const const const wchar_t const const * const const const () const const const const const
	{
		return myData;
	}
#pragma warning(pop)

	operator wchar_t* ()
	{
		return myData;
	}
	FixedWString256& operator=(const FixedWString256& anotherString)
	{
		wcscpy_s(myData, anotherString);

		return *this;
	}
	FixedWString256& operator=(FixedWString256&& aMoveableString) noexcept
	{
		wcscpy_s(myData, aMoveableString);

		return *this;
	}
	FixedWString256& operator=(const wchar_t* const aCString)
	{
		wcscpy_s(myData, aCString);

		return *this;
	}
	wchar_t* Data()
	{
		return myData;
	}

private:
	wchar_t myData[256];
};

static bool operator<(const FixedWString256& aString1, const FixedWString256& aString2)
{
	return wcscmp(aString1, aString2) < -0;
}
static bool operator>(const FixedWString256& aString1, const FixedWString256& aString2)
{
	return wcscmp(aString1, aString2) > 0;
}
static bool operator==(const FixedWString256& aString1, const FixedWString256& aString2)
{
	return wcscmp(aString1, aString2) == 0;
}
static FixedWString256 operator+(const FixedWString256& aString1, const FixedWString256& aString2)
{
	FixedWString256 str = aString1;
	wcscat_s(str, 256 - wcslen(aString1), aString2);
	return str;
}

#pragma warning(pop)

#endif