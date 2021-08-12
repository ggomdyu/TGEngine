#pragma once

#include "Version.h"
#include "PlatformID.h"

namespace tg
{

class OperatingSystem final
{
/**@section Constructor */
public:
    OperatingSystem(PlatformID platform, Version version) noexcept;
    OperatingSystem(PlatformID platform, Version version, std::u8string servicePack) noexcept;

/**@section Method */
public:
    [[nodiscard]] PlatformID GetPlatform() const noexcept;
    [[nodiscard]] const std::u8string& GetServicePack() const noexcept;
    [[nodiscard]] const Version& GetVersion() const noexcept;
    [[nodiscard]] const std::u8string& ToString() const;
    [[nodiscard]] const std::u8string& GetVersionString() const;

/**@section Variable */
private:
    PlatformID m_platform;
    Version m_version;
    std::u8string m_servicePack;
    mutable std::u8string m_versionString;
};

inline OperatingSystem::OperatingSystem(PlatformID platform, Version version) noexcept :
    OperatingSystem(platform, version, {})
{
}

inline OperatingSystem::OperatingSystem(PlatformID platform, Version version, std::u8string servicePack) noexcept :
    m_platform(platform),
    m_version(version),
    m_servicePack(std::move(servicePack))
{
}

inline PlatformID OperatingSystem::GetPlatform() const noexcept
{
    return m_platform;
}

inline const std::u8string& OperatingSystem::GetServicePack() const noexcept
{
    return m_servicePack;
}

inline const Version& OperatingSystem::GetVersion() const noexcept
{
    return m_version;
}

inline const std::u8string& OperatingSystem::ToString() const
{
    return this->GetVersionString();
}

inline const std::u8string& OperatingSystem::GetVersionString() const
{
    if (!m_versionString.empty())
    {
        return m_versionString;
    }

    std::u8string versionString;
    switch (m_platform)
    {
    case PlatformID::Win32NT:
        versionString = u8"Microsoft Windows NT ";
        break;

    case PlatformID::Win32Windows:
        versionString = (m_version.GetMajor() > 4) || ((m_version.GetMajor() == 4) && (m_version.GetMinor() > 0)) ? u8"Microsoft Windows 98 " : u8"Microsoft Windows 95 ";
        break;

    case PlatformID::Win32S:
        versionString = u8"Microsoft Win32S ";
        break;

    case PlatformID::WinCE:
        versionString = u8"Microsoft Windows CE ";
        break;

    case PlatformID::MacOSX:
        versionString = u8"Mac OS X ";
        break;

    default:
        versionString = u8"<unknown> ";
        break;
    }

    if (m_servicePack.empty())
    {
        versionString += m_version.ToString();
    }
    else
    {
        versionString += m_version.ToString(3) + u8" " + m_servicePack;
    }

    m_versionString = std::move(versionString);

    return m_versionString;
}

}