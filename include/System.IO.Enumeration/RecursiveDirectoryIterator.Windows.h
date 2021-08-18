#pragma once

#include <string>

#include "System/Windows/SafeFindHandle.h"

TG_NAMESPACE_BEGIN

class DirectoryIteratorImpl final
{
public:
    DirectoryIteratorImpl(std::u16string_view basePath, std::u16string_view searchPattern, uint32_t filter) noexcept;

public:
    void Advance();
    std::u16string_view Entry() noexcept;

private:
    static SafeFindHandle OpenHandle(std::u16string_view basePath, std::u16string_view searchPattern, WIN32_FIND_DATAW& findFileData) noexcept;
    static bool IsMatchWithPattern(std::wstring_view path, std::wstring_view searchPattern) noexcept;

private:
    SafeFindHandle handle_;
    WIN32_FIND_DATAW findData_;
    std::u16string basePath_;
    std::wstring searchPattern_;
    uint32_t filter_;
};

TG_NAMESPACE_END