#include "System.Text/EncoderReplacementFallback.h"

TG_NAMESPACE_BEGIN

EncoderReplacementFallback::EncoderReplacementFallback(std::u16string replacement) :
    replacement_(std::move(replacement))
{
}

void EncoderReplacementFallback::Fallback(ReadOnlySpan<char16_t> charsUnknown)
{
}

ReadOnlySpan<char16_t> EncoderReplacementFallback::GetChars() const noexcept
{
    return replacement_;
}

TG_NAMESPACE_END