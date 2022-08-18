#include "sce_pch.h"
#include "sce_log.h"

#pragma region LogW
sce::LogW::LogW(const wchar_t* name, int flags) : m_Flags(flags), m_Name(name), m_Path(), m_ConsoleStream(nullptr), m_FileStream(nullptr), m_Buffer()
{
	Setup(m_Flags);
}

sce::LogW::LogW(const wchar_t* name, const wchar_t* path, int flags) : m_Flags(flags), m_Name(name), m_Path(path), m_ConsoleStream(nullptr), m_FileStream(nullptr), m_Buffer()
{
	Setup(m_Flags);
}

sce::LogW::~LogW()
{
}

void sce::LogW::Print(const wchar_t* err_msg)
{
	m_Buffer.clear();

	FormatHeader();

	m_Buffer.append(err_msg);

	if (m_ConsoleStream)
	{
		*m_ConsoleStream << m_Buffer.c_str() << std::endl;
	}

	if (m_FileStream)
	{
		std::wstreambuf* szBuf = m_ConsoleStream->rdbuf();
		m_ConsoleStream->set_rdbuf(m_FileStream->rdbuf());

		*m_FileStream << m_Buffer.c_str() << std::endl;
		m_ConsoleStream->set_rdbuf(szBuf);
	}
}

const wchar_t* sce::LogW::GetName() const
{
	return m_Name.c_str();
}

const wchar_t* sce::LogW::GetPath() const
{
	return m_Path.c_str();
}

void sce::LogW::Setup(int flags)
{
	if (m_Flags & LogFlags::Stderr)
	{
		m_ConsoleStream = std::make_unique<std::wostream>(std::wcerr.rdbuf());
	}
	else if (m_Flags & LogFlags::Stdout)
	{
		m_ConsoleStream = std::make_unique<std::wostream>(std::wcout.rdbuf());
	}
	else if (m_Flags & LogFlags::Stdclog)
	{
		m_ConsoleStream = std::make_unique<std::wostream>(std::wclog.rdbuf());
	}

	if (m_Flags & LogFlags::Outfile && m_Path.size() > 0)
	{
		m_FileStream = std::make_unique<std::wofstream>(m_Path.c_str(), std::ios::out | std::ios::trunc);
	}
}

void sce::LogW::FormatHeader()
{
	if (m_Flags & LogFlags::Prefix)
	{
		m_Buffer.assign(m_Name.c_str(), m_Name.size());
	}

	if (m_Flags & LogFlags::TimeStamp)
	{
		std::time_t curr_time;
		std::tm* curr_tm;

		std::time(&curr_time);
		curr_tm = localtime(&curr_time);

		char datetime_str_a[30];

		if (!(m_Flags & LogFlags::Compact))
		{
			std::strftime(datetime_str_a, 30, "%d/%m/%Y %T", curr_tm);
		}
		else
		{
			std::strftime(datetime_str_a, 30, "%T", curr_tm);
		}

		if (m_Buffer.size() > 0)
		{
			m_Buffer.append(L" ");
		}

		wchar_t datetime_str_w[30];
		size_t nSize = 0;

		mbstowcs_s(&nSize, datetime_str_w, datetime_str_a, strlen(datetime_str_a) + 1);
		m_Buffer.append(datetime_str_w);
	}

#if defined(SCE_DEBUG)
	if (m_Flags & LogFlags::FileLine)
	{
		if (m_Buffer.size() > 0)
		{
			m_Buffer.append(L" ");
		}

		std::wstring szFile = __FILEW__;

		if (!(m_Flags & LogFlags::Compact))
		{
			m_Buffer.append(szFile.c_str());
		}
		else
		{
			size_t sz_size = szFile.find_last_of(L"\\") + 1;
			size_t sz_new = szFile.size() - sz_size;
			m_Buffer.append(szFile.substr(sz_size, sz_new));
			m_Buffer.append(L":");
		}

		size_t nSize = 20;
		int nLine = __LINE__;
		int nRad = 10;
		wchar_t* szBuf = new wchar_t[nSize];

		_itow_s(nLine, szBuf, nSize, nRad);
		m_Buffer.append(szBuf);
		
		if (szBuf)
		{
			delete[] szBuf;
		}
	}

	if (m_Flags & LogFlags::Function)
	{
		if (m_Buffer.size() > 0)
		{
			m_Buffer.append(L" ");
		}

		std::wstring szFunc = __FUNCTIONW__;

		if (m_Flags & LogFlags::Compact)
		{
			size_t nPos = szFunc.find_last_of(L':');
			size_t nSize = szFunc.size();
			szFunc = szFunc.substr(nPos + 1, nSize - (nSize - nPos - 1) + 1);
		}

		m_Buffer.append(szFunc.c_str());
	}
#endif //!SCE_DEBUG

	m_Buffer.append(L"-> ");
}
#pragma endregion LogW

#pragma region LogA
sce::LogA::LogA(const char* name, int flags) : m_Flags(flags), m_Name(name), m_Path(), m_ConsoleStream(nullptr), m_FileStream(nullptr), m_Buffer()
{
	Setup(m_Flags);
}

sce::LogA::LogA(const char* name, const char* path, int flags) : m_Flags(flags), m_Name(name), m_Path(path), m_ConsoleStream(nullptr), m_FileStream(nullptr), m_Buffer()
{
	Setup(m_Flags);
}

sce::LogA::~LogA()
{
}

void sce::LogA::Print(const char* err_msg)
{
	m_Buffer.clear();

	FormatHeader();

	m_Buffer.append(err_msg);

	if (m_ConsoleStream)
	{
		*m_ConsoleStream << m_Buffer.c_str() << std::endl;
	}

	if (m_FileStream)
	{
		std::streambuf* szBuf = m_ConsoleStream->rdbuf();
		m_ConsoleStream->set_rdbuf(m_FileStream->rdbuf());

		*m_FileStream << m_Buffer.c_str() << std::endl;
		m_ConsoleStream->set_rdbuf(szBuf);
	}
}

const char* sce::LogA::GetName() const
{
	return m_Name.c_str();
}

const char* sce::LogA::GetPath() const
{
	return m_Path.c_str();
}

void sce::LogA::Setup(int flags)
{
	if (m_Flags & LogFlags::Stderr)
	{
		m_ConsoleStream = std::make_unique<std::ostream>(std::cerr.rdbuf());
	}
	else if (m_Flags & LogFlags::Stdout)
	{
		m_ConsoleStream = std::make_unique<std::ostream>(std::cout.rdbuf());
	}
	else if (m_Flags & LogFlags::Stdclog)
	{
		m_ConsoleStream = std::make_unique<std::ostream>(std::clog.rdbuf());
	}

	if (m_Flags & LogFlags::Outfile && m_Path.size() > 0)
	{
		m_FileStream = std::make_unique<std::ofstream>(m_Path.c_str(), std::ios::out | std::ios::trunc);
	}
}

void sce::LogA::FormatHeader()
{
	if (m_Flags & LogFlags::Prefix)
	{
		m_Buffer.assign(m_Name.c_str(), m_Name.size());
	}

	if (m_Flags & LogFlags::TimeStamp)
	{
		std::time_t curr_time;
		std::tm* curr_tm;

		std::time(&curr_time);
		curr_tm = localtime(&curr_time);

		char datetime_str_a[30];

		if (!(m_Flags & LogFlags::Compact))
		{
			std::strftime(datetime_str_a, 30, "%d/%m/%Y %T", curr_tm);
		}
		else
		{
			std::strftime(datetime_str_a, 30, "%T", curr_tm);
		}

		if (m_Buffer.size() > 0)
		{
			m_Buffer.append(" ");
		}

		m_Buffer.append(datetime_str_a);
	}

#if defined(SCE_DEBUG)
	if (m_Flags & LogFlags::FileLine)
	{
		if (m_Buffer.size() > 0)
		{
			m_Buffer.append(" ");
		}

		std::string szFile = __FILE__;

		if (!(m_Flags & LogFlags::Compact))
		{
			m_Buffer.append(szFile.c_str());
		}
		else
		{
			size_t sz_size = szFile.find_last_of("\\") + 1;
			size_t sz_new = szFile.size() - sz_size;
			m_Buffer.append(szFile.substr(sz_size, sz_new));
			m_Buffer.append(":");
		}

		size_t nSize = 20;
		int nLine = __LINE__;
		int nRad = 10;
		char* szBuf = new char[nSize];

		_itoa_s(nLine, szBuf, nSize, nRad);
		m_Buffer.append(szBuf);

		if (szBuf)
		{
			delete[] szBuf;
		}
	}

	if (m_Flags & LogFlags::Function)
	{
		if (m_Buffer.size() > 0)
		{
			m_Buffer.append(" ");
		}

		std::string szFunc = __FUNCTION__;

		if (m_Flags & LogFlags::Compact)
		{
			size_t nPos = szFunc.find_last_of(L':');
			size_t nSize = szFunc.size();
			szFunc = szFunc.substr(nPos + 1, nSize - (nSize - nPos - 1) + 1);
		}

		m_Buffer.append(szFunc.c_str());
	}
#endif //!SCE_DEBUG

	m_Buffer.append("-> ");
}
#pragma endregion LogA

#if defined(SCE_UNICODE)
sce::Log::Log(const wchar_t* name, int flags) : LogW(name, flags)
{
}

sce::Log::Log(const wchar_t* name, const wchar_t* path, int flags) : LogW(name, path, flags)
{
}

sce::Log::~Log()
{
}
#elif //SCE_UNICODE
sce::Log::Log(const char* name, int flags) : Log(name, flags)
{
}

sce::Log::Log(const char* name, const char* path, int flags) : Log(name, path, flags)
{
}

sce::Log::~Log()
{
}
#endif //!SCE_UNICODE
