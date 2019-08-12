#include "EngineException.hpp"

const std::wstring& My::EngineException::GetNote() const
{
    return note;
}

const std::wstring& My::EngineException::GetFile() const
{
    return file;
}

unsigned int My::EngineException::GetLine() const
{
    return line;
}

std::wstring My::EngineException::GetLocation() const
{
    return std::wstring( L"Line [" ) + std::to_wstring( line ) + L"] in " + file;
}