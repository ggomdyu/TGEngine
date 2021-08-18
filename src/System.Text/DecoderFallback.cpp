#include "System.Text/DecoderFallback.h"
#include "System.Text/DecoderReplacementFallback.h"
#include "System.Text/DecoderExceptionFallback.h"

TG_NAMESPACE_BEGIN

std::shared_ptr<DecoderFallback> DecoderFallback::ReplacementFallback = std::make_shared<DecoderReplacementFallback>(u"?");
std::shared_ptr<DecoderFallback> DecoderFallback::ExceptionFallback = std::make_shared<DecoderExceptionFallback>();

ReadOnlySpan<char16_t> DecoderFallback::GetChars() const noexcept
{
    return {};
}

int32_t DecoderFallback::MaxCharCount() const noexcept
{
    return GetChars().Length();
}

TG_NAMESPACE_END