#import "PrecompiledHeader.pch"
#import "IOSTime.h"

#import "Core/Platform/Generic/GenericTime.h"

namespace tgon
{
namespace platform
{
namespace
{

//const uint32_t g_oldOSExecutionTime = GetOSExecutionTime();
//const uint64_t g_oldOSExecutionTime64 = GetOSExecutionTime64();

} /* namespace */

//TGON_API uint32_t GetProcessExecutionTime()
//{
//    return GetOSExecutionTime() - g_oldOSExecutionTime;
//}
//
//TGON_API uint64_t GetProcessExecutionTime64()
//{
//    return GetOSExecutionTime64() - g_oldOSExecutionTime64;
//}

TGON_API DateTime GetSystemTime()
{
    std::time_t unixTime = std::time(nullptr);

    std::tm rawUtcTime;
    gmtime_r(&unixTime, &rawUtcTime);

    return DateTime(rawUtcTime);
}

TGON_API DateTime GetLocalTime()
{
    std::time_t unixTime = std::time(nullptr);

    std::tm rawLocalTime;
    gmtime_r(&unixTime, &rawLocalTime);
    localtime_r(&unixTime, &rawLocalTime);

    return DateTime(rawLocalTime);
}

} /* namespace platform */
} /* namespace tgon */
