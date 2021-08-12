#include "PrecompiledHeader.h"

#include <cstdio>
#include <fstream>

#include "Time/TimeZoneInfo.h"

#include "File.h"
#include "FileStream.h"

namespace tg
{
namespace
{

[[nodiscard]] const Encoding& DetectEncoding(const std::byte* bytes, int32_t count) noexcept
{
    if (bytes != nullptr && count > 2)
    {
        if (bytes[0] == std::byte(0xff) && bytes[1] == std::byte(0xFE))
        {
            if (count < 4 || bytes[2] != std::byte(0) || bytes[3] != std::byte(0))
            {
                return Encoding::Unicode();
            }

            return Encoding::UTF32();
        }

        if (bytes[0] == std::byte(0xfe) && bytes[1] == std::byte(0xff))
        {
            return Encoding::BigEndianUnicode();
        }
        
        if (count >= 3 && bytes[0] == std::byte(0xef) && bytes[1] == std::byte(0xbb) && bytes[2] == std::byte(0xbf))
        {
            return Encoding::UTF8();
        }

        if (count >= 4 && bytes[0] == std::byte(0) && bytes[1] == std::byte(0) && bytes[2] == std::byte(0xfe) && bytes[3] == std::byte(0xff))
        {
            return *Encoding::GetEncoding(u8"UTF32BE");
        }
    }

    return Encoding::UTF8();
}

void RemoveByteOrderMark(std::u8string& str, const Encoding& encoding)
{
    const auto byteOrderMark = encoding.GetPreamble();
    if (byteOrderMark.size() > str.length())
    {
        return;
    }

    size_t removeCount = 0;
    for (size_t i = 0; i < byteOrderMark.size(); ++i)
    {
        if (byteOrderMark[i] != std::byte(str[i]))
        {
            break;
        }

        ++removeCount;
    }

    str.erase(str.begin(), str.begin() + removeCount);
}

}

std::optional<DateTime> File::GetCreationTime(const char8_t* path)
{
    auto ret = GetCreationTimeUtc(path);
    if (ret)
    {
        return ret.value() + TimeZoneInfo::Local().GetBaseUtcOffset();
    }
    
    return ret;
}

std::optional<DateTime> File::GetLastAccessTime(const char8_t* path)
{
    auto ret = GetLastAccessTimeUtc(path);
    if (ret)
    {
        return ret.value() + TimeZoneInfo::Local().GetBaseUtcOffset();
    }
    
    return ret;
}

std::optional<DateTime> File::GetLastWriteTime(const char8_t* path)
{
    auto ret = GetLastWriteTimeUtc(path);
    if (ret)
    {
        return ret.value() + TimeZoneInfo::Local().GetBaseUtcOffset();
    }

    return ret;
}

bool File::SetCreationTime(const char8_t* path, const DateTime& creationTime)
{
    return SetCreationTimeUtc(path, creationTime);
}

bool File::SetLastAccessTime(const char8_t* path, const DateTime& lastAccessTime)
{
    return SetLastAccessTimeUtc(path, lastAccessTime);
}

bool File::SetLastWriteTime(const char8_t* path, const DateTime& lastWriteTime)
{
    return SetLastWriteTimeUtc(path, lastWriteTime);
}

std::unique_ptr<std::byte[]> File::ReadAllBytes(const char8_t* path, ReturnPointerTag)
{
#ifdef _MSC_VER
    FILE* fp = nullptr;
    fopen_s(&fp, reinterpret_cast<const char*>(path), "rb");
#else
    FILE* fp = fopen(reinterpret_cast<const char*>(path), "rb");
#endif
    if (fp == nullptr)
    {
        return nullptr;
    }

    fseek(fp, 0, SEEK_END);
    const auto fileSize = static_cast<size_t>(ftell(fp));
    fseek(fp, 0, SEEK_SET);

    auto fileData = std::make_unique<std::byte[]>(static_cast<size_t>(fileSize));
#ifdef _MSC_VER
    fread_s(&fileData[0], fileSize, 1, fileSize, fp);
#else
    fread(&fileData[0], 1, fileSize, fp);
#endif

    fclose(fp);
    return fileData;
}

std::optional<std::vector<std::byte>> File::ReadAllBytes(const char8_t* path, ReturnVectorTag)
{
#ifdef _MSC_VER
    FILE* fp = nullptr;
    fopen_s(&fp, reinterpret_cast<const char*>(path), "rb");
#else
    FILE* fp = fopen(reinterpret_cast<const char*>(path), "rb");
#endif
    if (fp == nullptr)
    {
        return {};
    }

    fseek(fp, 0, SEEK_END);
    const auto fileSize = static_cast<size_t>(ftell(fp));
    fseek(fp, 0, SEEK_SET);

    std::vector<std::byte> fileData(fileSize);
#ifdef _MSC_VER
    fread_s(&fileData[0], fileSize, 1, fileSize, fp);
#else
    fread(&fileData[0], 1, fileSize, fp);
#endif

    fclose(fp);
    return fileData;
}

std::optional<std::u8string> File::ReadAllText(const char8_t* path)
{
#ifdef _MSC_VER
    FILE* fp = nullptr;
    fopen_s(&fp, reinterpret_cast<const char*>(path), "r");
#else
    FILE* fp = fopen(reinterpret_cast<const char*>(path), "r");
#endif
    if (fp == nullptr)
    {
        return {};
    }

    fseek(fp, 0, SEEK_END);
    const auto fileSize = static_cast<size_t>(ftell(fp));
    fseek(fp, 0, SEEK_SET);

    std::u8string fileData(fileSize, '\0');
#ifdef _MSC_VER
    fread_s(&fileData[0], fileSize, 1, fileSize, fp);
#else
    fread(&fileData[0], 1, fileSize, fp);
#endif

    fclose(fp);

    const auto& encoding = DetectEncoding(reinterpret_cast<const std::byte*>(fileData.c_str()), fileData.length());
    RemoveByteOrderMark(fileData, encoding);

    if (encoding != Encoding::UTF8())
    {
        return encoding.GetString(reinterpret_cast<const std::byte*>(fileData.data()), static_cast<int32_t>(fileData.length()));
    }
    
    return fileData;
}

std::optional<std::u8string> File::ReadAllText(const char8_t* path, const Encoding& encoding)
{
#ifdef _MSC_VER
    FILE* fp = nullptr;
    fopen_s(&fp, reinterpret_cast<const char*>(path), "r");
#else
    FILE* fp = fopen(reinterpret_cast<const char*>(path), "r");
#endif
    if (fp == nullptr)
    {
        return {};
    }

    fseek(fp, 0, SEEK_END);
    const auto fileSize = static_cast<size_t>(ftell(fp));
    fseek(fp, 0, SEEK_SET);

    std::u8string fileData(fileSize, '\0');
#ifdef _MSC_VER
    fread_s(&fileData[0], fileSize, 1, fileSize, fp);
#else
    fread(&fileData[0], 1, fileSize, fp);
#endif

    fclose(fp);

    RemoveByteOrderMark(fileData, encoding);

    if (encoding != Encoding::UTF8())
    {
        return encoding.GetString(reinterpret_cast<const std::byte*>(fileData.data()), static_cast<int32_t>(fileData.length()));
    }
    
    return fileData;
}

std::optional<std::vector<std::u8string>> File::ReadAllLines(const char8_t* path)
{
    std::vector<std::u8string> ret;
    ReadLines(path, [&](std::u8string&& item)
    {
        ret.push_back(std::move(item));
        return true;
    });

    return std::move(ret);
}

std::optional<FileStream> File::Create(const char8_t* path)
{
    return FileStream::Create(path, FileMode::Create, FileAccess::ReadWrite, FileShare::None);
}

std::optional<FileStream> File::Create(const char8_t* path, int32_t bufferSize)
{
    return FileStream::Create(path, FileMode::Create, FileAccess::ReadWrite, FileShare::None, bufferSize);
}

std::optional<FileStream> File::Create(const char8_t* path, int32_t bufferSize, FileOptions options)
{
    return FileStream::Create(path, FileMode::Create, FileAccess::ReadWrite, FileShare::None, bufferSize, options);
}

std::optional<FileStream> File::Open(const char8_t* path, FileMode mode)
{
    return Open(path, mode, mode == FileMode::Append ? FileAccess::Write : FileAccess::ReadWrite, FileShare::None);
}

std::optional<FileStream> File::Open(const char8_t* path, FileMode mode, FileAccess access)
{
    return Open(path, mode, access, FileShare::None);
}

std::optional<FileStream> File::Open(const char8_t* path, FileMode mode, FileAccess access, FileShare share)
{
    return FileStream::Create(path, mode, access, share);
}

bool File::WriteAllBytes(const char8_t* path, const std::span<std::byte>& bytes)
{
    auto fs = FileStream::Create(path, FileMode::Create, FileAccess::Write, FileShare::Read);
    if (fs.has_value() == false)
    {
        return false;
    }

    fs->Write(&bytes[0], bytes.size());

    return true;
}

}