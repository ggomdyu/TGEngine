#pragma once

#include "System.Text/DecoderFallback.h"

TG_NAMESPACE_BEGIN

class DecoderExceptionFallback :
    public DecoderFallback
{
public:
    void Fallback(ReadOnlySpan<std::byte> bytesUnknown) override;

private:
    static void Throw(ReadOnlySpan<std::byte> bytesUnknown);
};

TG_NAMESPACE_END