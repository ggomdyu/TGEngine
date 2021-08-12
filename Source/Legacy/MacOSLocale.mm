#include "PrecompiledHeader.h"

#include <Foundation/NSString.h>
#include <Foundation/NSLocale.h>
#include <cstring>

#include "../Locale.h"

namespace tg
{
    
int32_t GetLanguage(char* destStr, std::size_t destStrBufferLen)
{
    NSString* currentLanguageCode = [[NSLocale currentLocale] objectForKey:NSLocaleLanguageCode];
    [currentLanguageCode cStringUsingEncoding:NSUTF8StringEncoding];
    
    std::strncpy(destStr, currentLanguageCode.UTF8String, currentLanguageCode.length);
    
    return static_cast<std::int32_t>(currentLanguageCode.length);
}

}
