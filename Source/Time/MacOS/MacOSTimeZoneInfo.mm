#include "PrecompiledHeader.h"

#include <Foundation/Foundation.h>

#include "../TimeZoneInfo.h"

namespace tg
{
namespace
{

[[nodiscard]] TimeZoneInfo CreateLocal()
{
    NSTimeZone* localTimeZone = [NSTimeZone localTimeZone];
    NSLocale* currentLocale = [NSLocale currentLocale];
    
    const auto* id = [localTimeZone name].UTF8String;
    const auto* standardName = [localTimeZone localizedName:NSTimeZoneNameStyleStandard locale:currentLocale].UTF8String;
    const auto* daylightDisplayName = [localTimeZone localizedName:NSTimeZoneNameStyleDaylightSaving locale:currentLocale].UTF8String;
    const TimeSpan baseUtcOffset(TimeSpan::TicksPerSecond * [localTimeZone secondsFromGMT]);
    const bool supportsDaylightSavingTime = [localTimeZone isDaylightSavingTime];
    
    return TimeZoneInfo(reinterpret_cast<const char8_t*>(id), baseUtcOffset, reinterpret_cast<const char8_t*>(standardName), reinterpret_cast<const char8_t*>(standardName), reinterpret_cast<const char8_t*>(daylightDisplayName), supportsDaylightSavingTime);
}
    
}

const TimeZoneInfo& TimeZoneInfo::Local()
{
    static auto timeZoneInfo = CreateLocal();
    return timeZoneInfo;
}

}
