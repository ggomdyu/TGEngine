#pragma once

#include "Platform/Config.h"

namespace tg
{

class PosixPath
{
/**@section Constructor */
public:
    PosixPath() = delete;
    
/**@section Variable */
public:
    static constexpr char8_t DirectorySeparatorChar = '/';
    static constexpr char8_t AltDirectorySeparatorChar = '/';
    static constexpr char8_t VolumeSeparatorChar = '/';
    static constexpr char8_t PathSeparator = ':';
};

using PlatformPath = PosixPath;

}
