#include "PrecompiledHeader.h"

#include <array>
#include <execinfo.h>
#include <Foundation/Foundation.h>
#include <mach/mach_time.h>
#include <pthread.h>
#include <sys/utsname.h>
#include <unistd.h>

#include "../Environment.h"

namespace tg
{
namespace
{
    
int32_t GetSpecialDirectory(NSString* path, const std::u8string_view& postfix, char8_t* destStr, int32_t destStrBufferLen)
{
    auto pathStrLen = [path lengthOfBytesUsingEncoding:NSUTF8StringEncoding];
    if (pathStrLen + postfix.length() + 1 > destStrBufferLen)
    {
        return 0;
    }
    
    memcpy(destStr, path.UTF8String, pathStrLen);
    memcpy(&destStr[pathStrLen], postfix.data(), postfix.length() + 1);
    
    return static_cast<int32_t>(pathStrLen + postfix.length());
}

int32_t GetSpecialDirectory(NSSearchPathDirectory searchPathDirectory, NSSearchPathDomainMask pathDomainMask, const std::u8string_view& postfix, char8_t* destStr, int32_t destStrBufferLen)
{
    NSArray<NSString*>* paths = NSSearchPathForDirectoriesInDomains(searchPathDirectory, pathDomainMask, YES);
    if ([paths count] <= 0)
    {
        return 0;
    }
    
    return GetSpecialDirectory([paths objectAtIndex: 0], postfix, destStr, destStrBufferLen);
}
    
} /* namespace */

bool Environment::SetEnvironmentVariable(const char8_t* name, const char8_t* value)
{
    return setenv(reinterpret_cast<const char*>(name), reinterpret_cast<const char*>(value), true) == 0;
}

bool Environment::SetEnvironmentVariable(const char8_t* name, const char8_t* value, EnvironmentVariableTarget target)
{
    if (target == EnvironmentVariableTarget::Process)
    {
        return SetEnvironmentVariable(name, value);
    }

    return false;
}

std::optional<int32_t> Environment::GetEnvironmentVariable(const char8_t* name, char8_t* destStr, int32_t destStrBufferLen)
{
    const char* envValue = getenv(reinterpret_cast<const char*>(name));
    if (envValue == nullptr)
    {
        return {};
    }
    
    size_t envValueLen = strlen(envValue);
    if (envValueLen + 1 > destStrBufferLen)
    {
        return {};
    }
    
    memcpy(destStr, envValue, envValueLen + 1);
    
    return static_cast<int32_t>(envValueLen);
}

std::optional<int32_t> Environment::GetEnvironmentVariable(const char8_t* name, EnvironmentVariableTarget target, char8_t* destStr, int32_t destStrBufferLen)
{
    if (target == EnvironmentVariableTarget::Process)
    {
        return GetEnvironmentVariable(name, destStr, destStrBufferLen);
    }

    return {};
}

int32_t Environment::GetCurrentManagedThreadId()
{
    return static_cast<int32_t>(pthread_mach_thread_np(pthread_self()));
}

std::optional<int32_t> Environment::GetUserName(char8_t* destStr, int32_t destStrBufferLen)
{
    if (getlogin_r(reinterpret_cast<char*>(destStr), destStrBufferLen) != 0)
    {
        return {};
    }
    
    return static_cast<int32_t>(std::char_traits<char8_t>::length(destStr));
}

std::optional<int32_t> Environment::GetMachineName(char8_t* destStr, int32_t destStrBufferLen)
{
    if (gethostname(reinterpret_cast<char*>(destStr), destStrBufferLen) != 0)
    {
        return {};
    }
    
    utsname name;
    uname(&name);
    
    int32_t nodeNameLen = static_cast<int32_t>(strlen(name.nodename));
    if (nodeNameLen + 1 > destStrBufferLen)
    {
        return {};
    }
    
    memcpy(destStr, name.nodename, nodeNameLen + 1);
    
    return nodeNameLen;
}

std::optional<int32_t> Environment::GetUserDomainName(char8_t* destStr, int32_t destStrBufferLen)
{
    return GetMachineName(destStr, destStrBufferLen);
}

std::optional<int32_t> Environment::GetFolderPath(SpecialFolder folder, char8_t* destStr, int32_t destStrBufferLen)
{
    switch (folder)
    {
    case SpecialFolder::ApplicationData:
        return GetSpecialDirectory(NSHomeDirectory(), u8".config", destStr, destStrBufferLen);
            
    case SpecialFolder::CommonApplicationData:
        return GetSpecialDirectory(@"/usr/share", {u8"", 0}, destStr, destStrBufferLen);
       
    case SpecialFolder::Desktop:
    case SpecialFolder::DesktopDirectory:
        return GetSpecialDirectory(NSDesktopDirectory, NSUserDomainMask, {u8"", 0}, destStr, destStrBufferLen);
            
    case SpecialFolder::Fonts:
        return GetSpecialDirectory(NSLibraryDirectory, NSUserDomainMask, u8"/Fonts", destStr, destStrBufferLen);

    case SpecialFolder::Favorites:
        return GetSpecialDirectory(NSLibraryDirectory, NSUserDomainMask, u8"/Favorites", destStr, destStrBufferLen);
            
    case SpecialFolder::InternetCache:
        return GetSpecialDirectory(NSCachesDirectory, NSUserDomainMask, {u8"", 0}, destStr, destStrBufferLen);
            
    case SpecialFolder::ProgramFiles:
        return GetSpecialDirectory(@"/Applications", {u8"", 0}, destStr, destStrBufferLen);
            
    case SpecialFolder::System:
        return GetSpecialDirectory(@"/System", {u8"", 0}, destStr, destStrBufferLen);
        
    case SpecialFolder::UserProfile:
    case SpecialFolder::MyDocuments:
        return GetSpecialDirectory(NSHomeDirectory(), {u8"", 0}, destStr, destStrBufferLen);
    
    case SpecialFolder::MyMusic:
        return GetSpecialDirectory(NSMusicDirectory, NSUserDomainMask, {u8"", 0}, destStr, destStrBufferLen);
        
    case SpecialFolder::MyPictures:
        return GetSpecialDirectory(NSPicturesDirectory, NSUserDomainMask, {u8"", 0}, destStr, destStrBufferLen);
            
    default:
        return {};
    }
}

const std::u8string& Environment::GetCommandLine()
{
    static std::u8string commandLine = []()
    {
        std::u8string ret;
        for (NSString* commandLine in [[NSProcessInfo processInfo] arguments])
        {
            ret += reinterpret_cast<const char8_t*>([commandLine UTF8String]);
            ret += u8" ";
        }
        
        ret.pop_back();
        return ret;
    } ();
    
    return commandLine;
}

int64_t Environment::GetTickCount()
{
    return mach_absolute_time() * 0.000001f;
}

bool Environment::Is64BitProcess()
{
    return sizeof(intptr_t) == 8;
}

bool Environment::Is64BitOperatingSystem()
{
    return Is64BitProcess();
}

std::u8string_view Environment::GetNewLine()
{
    return u8"\n";
}

int32_t Environment::GetSystemPageSize()
{
    return static_cast<int32_t>(getpagesize());
}

void Environment::FailFast(const char8_t* message)
{
    printf("FailFast:\n%s", reinterpret_cast<const char*>(message));

    assert(false);
}

int32_t Environment::GetStackTrace(char8_t* destStr, int32_t destStrBufferLen)
{
    std::array<void*, 2048> addr;
    int numFrames = backtrace(addr.data(), static_cast<int>(addr.size()));
    if (numFrames <= 0)
    {
        return 0;
    }
    
    int32_t destStrLen = 0;
    char** symbols = backtrace_symbols(addr.data(), numFrames);
    for (int i = 0; i < numFrames; ++i)
    {
        destStrLen += sprintf(reinterpret_cast<char*>(&destStr[destStrLen]), "%s\n", symbols[i]);
    }
    
    free(symbols);
    
    return destStrLen;
}

}
