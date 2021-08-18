#pragma once

#include <string>

TG_NAMESPACE_BEGIN

class DirectoryIteratorImpl final
{
public:
    DirectoryIteratorImpl(std::u16string_view basePath, std::u16string_view searchPattern, uint32_t filter) noexcept;

public:

private:
    static std::unique_ptr<DIR, decltype(&closedir)> OpenHandle(std::u16string_view basePath) noexcept;

private:

};

TG_NAMESPACE_END