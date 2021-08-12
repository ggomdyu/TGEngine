#pragma once

namespace tg
{

class WindowsPath
{
/**@section Constructor */
public:
    WindowsPath() = delete;
    
/**@section Variable */
protected:
    static constexpr char8_t DirectorySeparatorChar = u8'\\';
    static constexpr char8_t AltDirectorySeparatorChar = u8'/';
    static constexpr char8_t VolumeSeparatorChar = u8':';
    static constexpr char8_t PathSeparator = u8';';
};

using PlatformPath = WindowsPath;

}