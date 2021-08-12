#include "PrecompiledHeader.h"

#include <array>

#include "Platform/Windows/Windows.h"
#include "Text/Encoding.h"

#include "../TimeZoneInfo.h"

namespace tg
{

const TimeZoneInfo& TimeZoneInfo::Local()
{
    static auto timeZoneInfo = []()
    {
        DYNAMIC_TIME_ZONE_INFORMATION tzi;
        const bool isSupportDaylightSavingTime = GetDynamicTimeZoneInformation(&tzi) != TIME_ZONE_ID_UNKNOWN;
        
        const TimeSpan baseUtcOffset(TimeSpan::TicksPerMinute * -tzi.Bias);

        const auto utf8Id = Encoding::Unicode().GetString(reinterpret_cast<const std::byte*>(tzi.TimeZoneKeyName), static_cast<int32_t>(std::char_traits<wchar_t>::length(tzi.TimeZoneKeyName) * 2));
        const auto utf8StandardName = Encoding::Unicode().GetString(reinterpret_cast<const std::byte*>(tzi.StandardName), static_cast<int32_t>(std::char_traits<wchar_t>::length(tzi.StandardName) * 2));
        const auto utf8DaylightDisplayName = Encoding::Unicode().GetString(reinterpret_cast<const std::byte*>(tzi.DaylightName), static_cast<int32_t>(std::char_traits<wchar_t>::length(tzi.DaylightName) * 2));

        return TimeZoneInfo(*utf8Id, baseUtcOffset, *utf8StandardName, *utf8StandardName, *utf8DaylightDisplayName, isSupportDaylightSavingTime);
    } ();
    return timeZoneInfo;
}

}