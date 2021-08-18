#pragma once

#include "System.Text/EncoderFallback.h"
#include "System/Span.h"

TG_NAMESPACE_BEGIN

struct EncoderExceptionFallback :
    public EncoderFallback
{
public:
    void Fallback(ReadOnlySpan<char16_t> charsUnknown) override;

private:
    void Throw(ReadOnlySpan<char16_t> charsUnknown);
};

TG_NAMESPACE_END