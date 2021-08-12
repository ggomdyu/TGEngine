#pragma once

#include <string>
#include <fmt/format.h>

namespace tg
{

struct Version final
{
/**@section Constructor */
public:
    constexpr Version(int32_t major, int32_t minor, int32_t build, int32_t revision) noexcept;
    constexpr Version(int32_t major, int32_t minor, int32_t build) noexcept;
    constexpr Version(int32_t major, int32_t minor) noexcept;
    
/**@section Operator */
public:
    constexpr bool operator==(const Version& rhs) const noexcept;
    constexpr bool operator!=(const Version& rhs) const noexcept;
    constexpr bool operator<(const Version& rhs) const noexcept;
    constexpr bool operator>(const Version& rhs) const noexcept;
    constexpr bool operator<=(const Version& rhs) const noexcept;
    constexpr bool operator>=(const Version& rhs) const noexcept;

/**@section Method */
public:
    [[nodiscard]] constexpr int32_t GetMajor() const noexcept;
    [[nodiscard]] constexpr int32_t GetMinor() const noexcept;
    [[nodiscard]] constexpr int32_t GetBuild() const noexcept;
    [[nodiscard]] constexpr int32_t GetRevision() const noexcept;
    [[nodiscard]] constexpr int16_t GetMajorRevision() const noexcept;
    [[nodiscard]] constexpr int16_t GetMinorRevision() const noexcept;
    [[nodiscard]] constexpr int32_t GetHashCode() const noexcept;
    [[nodiscard]] constexpr int32_t CompareTo(const Version& rhs) const noexcept;
    [[nodiscard]] std::u8string ToString() const;
    [[nodiscard]] std::u8string ToString(int32_t fieldCount) const;

/**@section Variable */
private:
    int32_t m_major = 0;
    int32_t m_minor = 0;
    int32_t m_build = -1;
    int32_t m_revision = -1;
};

constexpr Version::Version(int32_t major, int32_t minor, int32_t build, int32_t revision) noexcept :
    m_major(major),
    m_minor(minor),
    m_build(build),
    m_revision(revision)
{
}

constexpr Version::Version(int32_t major, int32_t minor, int32_t build) noexcept :
    m_major(major),
    m_minor(minor),
    m_build(build)
{
}

constexpr Version::Version(int32_t major, int32_t minor) noexcept :
    m_major(major),
    m_minor(minor)
{
}

constexpr bool Version::operator==(const Version& rhs) const noexcept
{
    return this->CompareTo(rhs) == 0;
}

constexpr bool Version::operator!=(const Version& rhs) const noexcept
{
    return !this->operator==(rhs);
}

constexpr bool Version::operator<(const Version& rhs) const noexcept
{
    return this->CompareTo(rhs) < 0;
}

constexpr bool Version::operator<=(const Version& rhs) const noexcept
{
    return this->CompareTo(rhs) <= 0;
}

constexpr bool Version::operator>(const Version& rhs) const noexcept
{
    return rhs < *this;
}

constexpr bool Version::operator>=(const Version& rhs) const noexcept
{
    return rhs <= *this;
}

constexpr int32_t Version::GetMajor() const noexcept
{
    return m_major;
}

constexpr int32_t Version::GetMinor() const noexcept
{
    return m_minor;
}

constexpr int32_t Version::GetBuild() const noexcept
{
    return m_build;
}

constexpr int32_t Version::GetRevision() const noexcept
{
    return m_revision;
}

constexpr int16_t Version::GetMajorRevision() const noexcept
{
    return m_revision >> 16;
}

constexpr int16_t Version::GetMinorRevision() const noexcept
{
    return m_revision & 0xffff;
}

constexpr int32_t Version::GetHashCode() const noexcept
{
    int32_t accumulator = 0;
    accumulator |= (m_major & 0x0000000F) << 28;
    accumulator |= (m_minor & 0x000000FF) << 20;
    accumulator |= (m_build & 0x000000FF) << 12;
    accumulator |= (m_revision & 0x00000FFF);

    return accumulator;
}

constexpr int32_t Version::CompareTo(const Version& rhs) const noexcept
{
    if (m_major != rhs.m_major)
    {
        return m_major > rhs.m_major ? 1 : -1;
    }

    if (m_minor != rhs.m_minor)
    {
        return m_minor > rhs.m_minor ? 1 : -1;
    }

    if (m_build != rhs.m_build)
    {
        return m_build > rhs.m_build ? 1 : -1;\
    }

    if (m_revision != rhs.m_revision)
    {
        return m_revision > rhs.m_revision ? 1 : -1;
    }

    return 0;
}

inline std::u8string Version::ToString() const
{
    if (m_build == -1)
    {
        return this->ToString(2);
    }
    
    if (m_revision == -1)
    {
        return this->ToString(3);
    }

    return this->ToString(4);
}

inline std::u8string Version::ToString(int32_t fieldCount) const
{
    switch (fieldCount)
    {
    case 0:
        return {};
    
    case 1:
        return fmt::format(u8"{}", m_major);

    case 2:
        {
            return fmt::format(u8"{}.{}", m_major, m_minor);
        }

    default:
        {
            if (m_build == -1)
            {
                return {};
            }

            if (fieldCount == 3)
            {
                return fmt::format(u8"{}.{}.{}", m_major, m_minor, m_build);
            }

            if (m_revision == -1)
            {
                return {};
            }

            if (fieldCount == 4)
            {
                return fmt::format(u8"{}.{}.{}.{}", m_major, m_minor, m_build, m_revision);
            }

            return {};
        }
    }
}

}
