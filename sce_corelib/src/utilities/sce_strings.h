#pragma once

#include "system/sce_platform.h"

#include <vector>
#include <sstream>
#include <iostream>
#include <format>

namespace sce::utils
{
	template<class ... Args>
	std::vector<std::wstring> wstring_vector(Args ... args)
	{
		const auto print_impl = [](const auto& t)
		{
			std::wstringstream ss;
			ss << t;
			return ss.str();
		};

		return { print_impl(args)... };
	}

	template<class ... Args>
	std::vector<std::string> string_vector(Args ... args)
	{
		const auto print_impl = [](const auto& t)
		{
			std::stringstream ss;
			ss << t;
			return ss.str();
		};

		return { print_impl(args)... };;
	}

	template<class ... Args>
	errno_t print_w(wchar_t** buf, const Args& ... args)
	{
		std::vector<std::wstring> svec = wstring_vector(args...);

		std::wstringstream sstr;
		for (int i = 0; i < svec.size(); i++)
		{
			sstr << svec.at(i);
		}

		std::size_t sz = sstr.str().size();

		if (!*buf)
		{
			std::size_t sz = sstr.str().size();
			*buf = new wchar_t[sz + 1];
		}	

		return wcscpy_s(*buf, sz + 1, sstr.str().c_str());
	}

	template<typename ... Params>
	errno_t print_a(char* buf, const Params& ... params)
	{
		const auto print_impl = [](const auto& t)
		{
			std::stringstream ss;
			ss << t;
			return ss.str();
		};

		std::vector<std::string> svec;
		svec = { print_impl(params)... };

		std::stringstream sstr;
		for (int i = 0; i < svec.size(); i++)
		{
			sstr << svec.at(i);
		}

		return strcpy_s(buf, sstr.str().size() + 1, sstr.str().c_str());
	}

	template<class ... Args>
	errno_t printf_w(wchar_t* buf, std::wstring_view str_fmt, Args&& ... args)
	{
		std::wstring str = std::vformat(str_fmt, std::make_wformat_args(args...));
		std::size_t sz = str.size();
		return wcscpy_s(buf, sz + 1, str.c_str());
	}

	template<class ... Args>
	errno_t printf_a(char* buf, std::string_view str_fmt, Args&& ... args)
	{
		std::string str = std::vformat(str_fmt, std::make_format_args(args...));
		std::size_t sz = str.size();
		return strcpy_s(buf, sz + 1, str.c_str());
	}

#ifdef SCE_UNICODE
	template<typename ... Params>
	const wchar_t* print(wchar_t* buf, const Params& ... params)
	{
		return print_w(buf, params...);
	}
#else
	template<typename ... Params>
	const char* print(char* buf, const Params& ... params)
	{
		return print_a(buf, params...);
	}
#endif // SCE_UNICODE
}
