#pragma once

namespace tg
{


constexpr std::u8string_view Path::GetPathRoot(const std::u8string_view& path) noexcept
{
    //int len = 2;

    //if (path.Length == 1 && IsDirectorySeparator (path [0]))
    //    return DirectorySeparatorStr;
    //else if (path.Length < 2)
    //    return String.Empty;

    //if (IsDirectorySeparator (path [0]) && IsDirectorySeparator (path[1])) {
    //    // UNC: \\server or \\server\share
    //    // Get server
    //    while (len < path.Length && !IsDirectorySeparator (path [len])) len++;

    //    // Get share
    //    if (len < path.Length) {
    //        len++;
    //        while (len < path.Length && !IsDirectorySeparator (path [len])) len++;
    //    }

    //    return DirectorySeparatorStr +
    //        DirectorySeparatorStr +
    //        path.Substring (2, len - 2).Replace (AltDirectorySeparatorChar, DirectorySeparatorChar);
    //} else if (IsDirectorySeparator (path [0])) {
    //    // path starts with '\' or '/'
    //    return DirectorySeparatorStr;
    //} else if (path[1] == VolumeSeparatorChar) {
    //    // C:\folder
    //    if (path.Length >= 3 && (IsDirectorySeparator (path [2]))) len++;
    //} else
    //    return Directory.GetCurrentDirectory ().Substring (0, 2);// + path.Substring (0, len);
    //return path.Substring (0, len);
    return u8"";
}

constexpr int32_t Path::GetRootLength(const std::u8string_view& path) noexcept
{
    return path.length() > 0 && IsDirectorySeparator(path[0]) ? 1 : 0;
}

}
