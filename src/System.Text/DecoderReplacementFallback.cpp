#include "System.Text/DecoderReplacementFallback.h"

TG_NAMESPACE_BEGIN

DecoderReplacementFallback::DecoderReplacementFallback(std::u16string replacement) :
    replacement_(std::move(replacement))
{
}

void DecoderReplacementFallback::Fallback(ReadOnlySpan<std::byte> bytesUnknown)
{
}

ReadOnlySpan<char16_t> DecoderReplacementFallback::GetChars() const noexcept
{
    return replacement_;
}

TG_NAMESPACE_END