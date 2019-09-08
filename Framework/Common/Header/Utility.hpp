#pragma once
#include <sstream>
#include <iostream>

namespace My
{
	template <typename T>
	std::string Str(const T& t)
	{
		std::stringstream ss;
		ss << t;
		return ss.str();
	}

    template <typename T>
	std::wstring wStr(const T& t)
	{
		std::wstringstream ss;
		ss << t;
		return ss.str();
	}

	template<class T>
	inline void SafeRelease(T **ppInterfaceToRelease)
	{
		if (*ppInterfaceToRelease != nullptr)
		{
			(*ppInterfaceToRelease)->Release();

			(*ppInterfaceToRelease) = nullptr;
		}
	}
}
