#pragma once

#include "system/sce_platform.h"

#if not defined(_OSTREAM_)
	#include <ostream>
#endif //!_OSTREAM_

#if not defined(_FSTREAM_)
	#include <fstream>
#endif //!_FSTREAM_

#if not defined(_MEMORY_)
	#include <memory>
#endif //!_MEMORY_

#if not defined(_CTIME_)
	#include <ctime>
#endif //!_CTIME_

namespace sce
{
	enum LogFlags
	{
		// Create
		Stderr = (1 << 0),
		Stdout = (1 << 1),
		Stdclog = (1 << 2),
		Outfile = (1 << 3),

		// Write
		Prefix = (1 << 4),
		TimeStamp = (1 << 5),
#if defined(SCE_DEBUG)
		FileLine = (1 << 6),
		Function = (1 << 7),
#endif //!SCE_DEBUG
		Compact = (1 << 8),
	};

	class SCE_API LogW
	{
	private:
		int m_Flags;
		std::wstring m_Name;
		std::wstring m_Path;
		std::unique_ptr<std::wostream> m_ConsoleStream;
		std::unique_ptr<std::wofstream> m_FileStream;
		std::wstring m_Buffer;

	public:
		LogW(const wchar_t* name, int flags);
		LogW(const wchar_t* name, const wchar_t* path, int flags);
		~LogW();

		virtual void Print(const wchar_t* err_msg);

		virtual const wchar_t* GetName() const;
		virtual const wchar_t* GetPath() const;

	private:
		virtual void Setup(int flags);
		virtual void FormatHeader();
	};

	class SCE_API LogA
	{
	private:
		int m_Flags;
		std::string m_Name;
		std::string m_Path;
		std::unique_ptr<std::ostream> m_ConsoleStream;
		std::unique_ptr<std::ofstream> m_FileStream;
		std::string m_Buffer;

	public:
		LogA(const char* name, int flags);
		LogA(const char* name, const char* path, int flags);
		~LogA();

		virtual void Print(const char* err_msg);

		virtual const char* GetName() const;
		virtual const char* GetPath() const;

	private:
		virtual void Setup(int flags);
		virtual void FormatHeader();
	};

#if defined(SCE_UNICODE)
	class SCE_API Log : public LogW
	{
	public:
		Log(const wchar_t* name, int flags);
		Log(const wchar_t* name, const wchar_t* path, int flags);
		~Log();
	};
#elif //SCE_UNICODE
	class SCE_API Log : LogA
	{
	public:
		Log(const char* name, int flags);
		Log(const char* name, const char* path, int flags);
		~Log();
	};
#endif //!SCE_UNICODE
}