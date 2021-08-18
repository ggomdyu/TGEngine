#pragma once

#include <optional>
#include <string>

#include "System/Span.h"

TG_NAMESPACE_BEGIN

class TextReader
{
public:
    virtual ~TextReader() = default;

public:
    virtual int32_t Peek() = 0;
    virtual int32_t Read() = 0;
    virtual int32_t Read(Span<char16_t> bytes);
    virtual std::optional<std::u16string> ReadLine();
    virtual std::u16string ReadToEnd();
    virtual void Close() = 0;
};

TG_NAMESPACE_END
