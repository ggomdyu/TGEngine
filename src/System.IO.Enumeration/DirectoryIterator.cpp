#include "System.IO.Enumeration/DirectoryIterator.h"
#if TG_PLATFORM_WINDOWS
#include "System.IO.Enumeration/DirectoryIterator.Windows.h"
#else
#include "System.IO.Enumeration/DirectoryIterator.Posix.h"
#endif

TG_NAMESPACE_BEGIN

DirectoryIterator::DirectoryIterator(std::u16string_view basePath, std::u16string_view searchPattern, uint32_t filter) noexcept :
    impl_(std::make_shared<DirectoryIteratorImpl>(basePath, searchPattern, filter))
{
    impl_->Advance();
}

bool DirectoryIterator::operator==(const DirectoryIterator& rhs) noexcept
{
    return impl_ == rhs.impl_;
}

bool DirectoryIterator::operator!=(const DirectoryIterator& rhs) noexcept
{
    return !operator==(rhs);
}

std::u16string_view DirectoryIterator::operator*() noexcept
{
    return impl_->Entry();
}

std::u16string_view DirectoryIterator::operator->() noexcept
{
    return impl_->Entry();
}

DirectoryIterator& DirectoryIterator::operator++()
{
    impl_->Advance();
    return *this;
}

std::u16string_view DirectoryIterator::operator++(int)
{
    std::u16string_view entry = **this;
    ++*this;
    return entry;
}

TG_NAMESPACE_END