#pragma once

#include <memory>

#include "System/ReadOnlySpan.h"

TG_NAMESPACE_BEGIN

class DecoderFallback
{
public:
    virtual ~DecoderFallback() = default;

public:
    virtual void Fallback(ReadOnlySpan<std::byte> bytesUnknown) = 0;
    virtual ReadOnlySpan<char16_t> GetChars() const noexcept;
    int32_t MaxCharCount() const noexcept;

public:
    static std::shared_ptr<DecoderFallback> ReplacementFallback;
    static std::shared_ptr<DecoderFallback> ExceptionFallback;
};

TG_NAMESPACE_END