#pragma once

#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#include "Windows/WindowsConfig.h"

#elif defined(__APPLE__)
#if defined(TARGET_OS_MAC) || defined(macintosh) || defined(Macintosh) || defined(__MACH__)
#include "MacOS/MacOSConfig.h"
#elif defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
#include "IOS/IOSConfig.h"
#endif

#elif defined(__ANDROID__) || defined(__ANDROID_API__)
#include "Android/AndroidConfig.h"
#endif
