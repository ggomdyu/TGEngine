#pragma once

#include <memory>

#include "System/ReadOnlySpan.h"

TG_NAMESPACE_BEGIN

class EncoderFallback
{
public:
    virtual ~EncoderFallback() = default;

public:
    virtual void Fallback(ReadOnlySpan<char16_t> charsUnknown) = 0;
    virtual ReadOnlySpan<char16_t> GetChars() const noexcept;

public:
    static std::shared_ptr<EncoderFallback> ReplacementFallback;
    static std::shared_ptr<EncoderFallback> ExceptionFallback;
};

TG_NAMESPACE_END