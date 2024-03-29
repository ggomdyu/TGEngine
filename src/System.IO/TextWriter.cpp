#include "System.IO/TextWriter.h"
#include "System/Environment.h"

TG_NAMESPACE_BEGIN

TextWriter::TextWriter() :
    newLine_(Environment::NewLine())
{
}

void TextWriter::Write(std::u16string_view value)
{
    WriteCore(value, false);
}

void TextWriter::Write(const char16_t* value)
{
    Write(std::u16string_view(value));
}

void TextWriter::Write(bool value)
{
    Write(std::u16string_view(value ? u"True" : u"False"));
}

void TextWriter::WriteLine(std::u16string_view value)
{
    WriteCore(value, true);
}

void TextWriter::WriteLine(const char16_t* value)
{
    WriteLine(std::u16string_view(value));
}

void TextWriter::WriteLine()
{
    WriteLine(std::u16string_view());
}

void TextWriter::WriteLine(bool value)
{
    WriteLine(std::u16string_view(value ? u"True" : u"False"));
}

void TextWriter::SetNewLine(std::u16string_view newLine)
{
    newLine_ = newLine;
}

std::u16string_view TextWriter::GetNewLine() const noexcept
{
    return newLine_;
}

TG_NAMESPACE_END
