#pragma once

#include <memory>
#include <string>
#if !TG_PLATFORM_WINDOWS
#include <dirent.h>
#else
#include "System/Windows/Windows.h"
#endif

TG_NAMESPACE_BEGIN

struct DirectoryIterator final
{
public:
    explicit DirectoryIterator(std::u16string_view basePath, std::u16string_view searchPattern = u"*", uint32_t filter = std::numeric_limits<uint32_t>::max()) noexcept;

public:
    bool operator==(const DirectoryIterator& rhs) noexcept;
    bool operator!=(const DirectoryIterator& rhs) noexcept;
    std::u16string_view operator*() noexcept;
    std::u16string_view operator->() noexcept;
    DirectoryIterator& operator++();
    const std::u16string_view operator++(int);

private:
    void Advance();
    std::u16string_view Entry() noexcept;

private:
#if TG_PLATFORM_WINDOWS
    WIN32_FIND_DATAW findData_{};
    std::unique_ptr<void, void(*)(void*)> handle_;
    std::u16string buffer_;
    std::u16string basePath_;
    std::wstring searchPattern_;
#else
    std::unique_ptr<DIR, decltype(&closedir)> dir_;
    std::string basePath_;
    std::string searchPattern_;
#endif
    uint32_t filter_{};
};

inline DirectoryIterator begin(const DirectoryIterator& it) noexcept
{
    return it;
}

inline DirectoryIterator end(const DirectoryIterator&) noexcept
{
    return DirectoryIterator{};
}

TG_NAMESPACE_END