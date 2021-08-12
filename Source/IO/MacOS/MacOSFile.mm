#include "PrecompiledHeader.h"

#include <Foundation/Foundation.h>
#include <sys/stat.h>
#include <utime.h>
#include <cstdio>

#include "../File.h"

namespace tg
{
namespace
{

NSDate* ConvertDateTimeToNative(const DateTime& dateTime)
{
    NSDateComponents* dateComponents = [[NSDateComponents alloc] init];
    [dateComponents setYear:dateTime.GetYear()];
    [dateComponents setMonth:dateTime.GetMonth()];
    [dateComponents setDay:dateTime.GetDay()];
    [dateComponents setHour:dateTime.GetHour()];
    [dateComponents setMinute:dateTime.GetMinute()];
    [dateComponents setSecond:dateTime.GetSecond()];

    NSCalendar* calendar = [NSCalendar currentCalendar];

    NSDate* modificationDate = [[calendar dateFromComponents:dateComponents] dateByAddingTimeInterval:[[NSTimeZone systemTimeZone] secondsFromGMT]];
    return modificationDate;
}

}
    
bool File::SetCreationTimeUtc(const char8_t* path, const DateTime& creationTimeUtc)
{
    NSDate* date = ConvertDateTimeToNative(creationTimeUtc);
    NSDictionary* attributes = [NSDictionary dictionaryWithObjectsAndKeys: date, NSFileCreationDate, nullptr];
    
    NSFileManager* fileManager = [NSFileManager defaultManager];
    bool isSucceed = [fileManager setAttributes: attributes ofItemAtPath: [NSString stringWithUTF8String:reinterpret_cast<const char*>(path)] error: nullptr];
    return isSucceed;
}
    
bool File::SetLastAccessTimeUtc(const char8_t* path, const DateTime& lastAccessTimeUtc)
{
    int64_t ticks = (lastAccessTimeUtc.ToUniversalTime().GetTicks() - DateTime::GetUnixEpoch().GetTicks()) / TimeSpan::TicksPerSecond;
    
    utimbuf buf{ticks, ticks};
    return utime(reinterpret_cast<const char*>(path), &buf) == 0;
}

bool File::SetLastWriteTimeUtc(const char8_t* path, const DateTime& lastWriteTimeUtc)
{
    NSDate* dateComponents = ConvertDateTimeToNative(lastWriteTimeUtc);
    
    NSDictionary* attributes = [NSDictionary dictionaryWithObjectsAndKeys: dateComponents, NSFileModificationDate, nullptr];
    return [[NSFileManager defaultManager] setAttributes:attributes ofItemAtPath:[NSString stringWithUTF8String:reinterpret_cast<const char*>(path)] error:nil];
}

bool File::Copy(const char8_t* srcPath, const char8_t* destPath, bool overwrite)
{
    if (overwrite)
    {
        Delete(destPath);
    }
    
    NSString* nsSrcPath = [NSString stringWithUTF8String:reinterpret_cast<const char*>(srcPath)];
    NSString* nsDestPath = [NSString stringWithUTF8String:reinterpret_cast<const char*>(destPath)];
    return [[NSFileManager defaultManager] copyItemAtPath:nsSrcPath toPath:nsDestPath error:nil];
}

}
