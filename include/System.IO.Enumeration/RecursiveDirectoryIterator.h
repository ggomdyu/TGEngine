#pragma once

#include <memory>
#include <string>

TG_NAMESPACE_BEGIN

struct RecursiveDirectoryIterator final
{
public:
    constexpr RecursiveDirectoryIterator() = default;
    explicit RecursiveDirectoryIterator(std::u16string_view basePath, std::u16string_view searchPattern = u"*", uint32_t filter = std::numeric_limits<uint32_t>::max()) noexcept;

public:
    bool operator==(const RecursiveDirectoryIterator& rhs) noexcept;
    bool operator!=(const RecursiveDirectoryIterator& rhs) noexcept;
    std::u16string_view operator*() noexcept;
    std::u16string_view operator->() noexcept;
    RecursiveDirectoryIterator& operator++();
    std::u16string_view operator++(int);

private:
    std::shared_ptr<class DirectoryIteratorImpl> impl_;
};

inline RecursiveDirectoryIterator begin(const RecursiveDirectoryIterator& it) noexcept
{
    return it;
}

inline RecursiveDirectoryIterator end(const RecursiveDirectoryIterator&) noexcept
{
    return RecursiveDirectoryIterator{};
}

TG_NAMESPACE_END