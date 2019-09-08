#pragma once
#include <string>

namespace My
{
	class EngineException
	{
	public:
		EngineException() = delete;
		explicit EngineException( const wchar_t* file,unsigned int line,const std::wstring& note = L"" )
			:
			file( file ), //_CRT_WIDE(__FILE__)
			line( line ), //__LINE__,
			note( note )  //L"Error!"
		{}
		const std::wstring& GetNote() const;
		const std::wstring& GetFile() const;
		unsigned int GetLine() const;
		std::wstring GetLocation() const;
		virtual std::wstring GetFullMessage() const;
		virtual std::wstring GetExceptionType() const;
	private:
		std::wstring note;
		std::wstring file;
		unsigned int line;
	};
}
