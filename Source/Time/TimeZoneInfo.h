#pragma once

#include <string>

#include "DateTime.h"

namespace tg
{

class TimeZoneInfo final
{
/**@section Constructor */
public:
    TimeZoneInfo() noexcept = default;
    TimeZoneInfo(std::u8string id, const TimeSpan& baseUtcOffset, std::u8string displayName, std::u8string standardDisplayName, std::u8string daylightDisplayName, bool supportsDaylightSavingTime) noexcept;

/**@section Method */
public:
    [[nodiscard]] static const TimeZoneInfo& Local();
    [[nodiscard]] static const TimeZoneInfo& Utc() noexcept;
    [[nodiscard]] static DateTime ConvertTime(const DateTime& dateTime, const TimeZoneInfo& destinationTimeZone);
    [[nodiscard]] static DateTime ConvertTimeFromUtc(const DateTime& dateTime, const TimeZoneInfo& destinationTimeZone);
    [[nodiscard]] static DateTime ConvertTimeToUtc(const DateTime& dateTime);
    [[nodiscard]] const std::u8string& GetId() const noexcept;
    [[nodiscard]] const TimeSpan& GetBaseUtcOffset() const noexcept;
    [[nodiscard]] const std::u8string& GetStandardDisplayName() const noexcept;
    [[nodiscard]] const std::u8string& GetDaylightDisplayName() const noexcept;
    [[nodiscard]] bool IsSupportDaylightSavingTime() const noexcept;
    
private:
    [[nodiscard]] static DateTimeKind GetCorrespondingKind(const TimeZoneInfo& timeZone);
    [[nodiscard]] static DateTime ConvertTime(const DateTime& dateTime, const TimeZoneInfo& sourceTimeZone, const TimeZoneInfo& destinationTimeZone);

/**@section Variable */
private:
    std::u8string m_id;
    TimeSpan m_baseUtcOffset = TimeSpan(0);
    std::u8string m_displayName;
    std::u8string m_standardDisplayName;
    std::u8string m_daylightDisplayName;
    bool m_supportsDaylightSavingTime = false;
};

inline TimeZoneInfo::TimeZoneInfo(std::u8string id, const TimeSpan& baseUtcOffset, std::u8string displayName, std::u8string standardDisplayName, std::u8string daylightDisplayName, bool supportsDaylightSavingTime) noexcept :
    m_id(std::move(id)),
    m_baseUtcOffset(baseUtcOffset),
    m_displayName(std::move(displayName)),
    m_standardDisplayName(std::move(standardDisplayName)),
    m_daylightDisplayName(std::move(daylightDisplayName)),
    m_supportsDaylightSavingTime(supportsDaylightSavingTime)
{
}

inline const TimeZoneInfo& TimeZoneInfo::Utc() noexcept
{
    static auto timeZoneInfo = []()
    {
        const std::u8string id = u8"UTC";
        return TimeZoneInfo(id, TimeSpan(0), id, id, id, false);
    } ();
    return timeZoneInfo;
}

inline DateTime TimeZoneInfo::ConvertTime(const DateTime& dateTime, const TimeZoneInfo& destinationTimeZone)
{
    if (dateTime.GetKind() == DateTimeKind::Utc)
    {
        return ConvertTime(dateTime, Utc(), destinationTimeZone);
    }

    return ConvertTime(dateTime, Local(), destinationTimeZone);
}

inline DateTime TimeZoneInfo::ConvertTimeFromUtc(const DateTime& dateTime, const TimeZoneInfo& destinationTimeZone)
{
    return ConvertTime(dateTime, TimeZoneInfo::Utc(), destinationTimeZone);
}

inline DateTime TimeZoneInfo::ConvertTimeToUtc(const DateTime& dateTime)
{
    return ConvertTime(dateTime, TimeZoneInfo::Local(), TimeZoneInfo::Utc());
}

inline const std::u8string& TimeZoneInfo::GetId() const noexcept
{
    return m_id;
}

inline const TimeSpan& TimeZoneInfo::GetBaseUtcOffset() const noexcept
{
    return m_baseUtcOffset;
}

inline const std::u8string& TimeZoneInfo::GetStandardDisplayName() const noexcept
{
    return m_standardDisplayName;
}

inline const std::u8string& TimeZoneInfo::GetDaylightDisplayName() const noexcept
{
    return m_daylightDisplayName;
}

inline bool TimeZoneInfo::IsSupportDaylightSavingTime() const noexcept
{
    return m_supportsDaylightSavingTime;
}

inline DateTimeKind TimeZoneInfo::GetCorrespondingKind(const TimeZoneInfo& timeZone)
{
    if (&timeZone == &Utc())
    {
        return DateTimeKind::Utc;
    }

    if (&timeZone == &Local())
    {
        return DateTimeKind::Local;
    }
    
    return DateTimeKind::Unspecified;
}

inline DateTime TimeZoneInfo::ConvertTime(const DateTime& dateTime, const TimeZoneInfo& sourceTimeZone, const TimeZoneInfo& destinationTimeZone)
{
    // The kind of dateTime and sourceTimeZone must be the same.
    const auto sourceKind = GetCorrespondingKind(sourceTimeZone);
    if (dateTime.GetKind() != sourceKind)
    {
        return dateTime;
    }

    // Filter the special case like UTC->UTC or Local->Local
    const auto destinationKind = GetCorrespondingKind(destinationTimeZone);
    if (sourceKind != DateTimeKind::Unspecified && destinationKind != DateTimeKind::Unspecified && sourceKind == destinationKind)
    {
        return dateTime;
    }

    // Convert the dateTime utc offset to 0.
    const auto ticks = dateTime.GetTicks() - sourceTimeZone.GetBaseUtcOffset().GetTicks();
    if (destinationKind == DateTimeKind::Local)
    {
        return DateTime(ticks + destinationTimeZone.GetBaseUtcOffset().GetTicks(), DateTimeKind::Local);
    }

    return DateTime(ticks, destinationKind);
}

}
