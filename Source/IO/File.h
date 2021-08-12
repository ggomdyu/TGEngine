#pragma once

#include <memory>
#include <optional>

#include "Core/TagDispatch.h"
#include "Text/Encoding.h"
#include "Time/DateTime.h"

#include "FileAttributes.h"
#include "FileStream.h"

namespace tg
{

class File final
{
/**@section Constructor */
public:
    File() = delete;

/**@section Method */
public:
    static bool Copy(const char8_t* srcPath, const char8_t* destPath, bool overwrite = false);
    static bool Delete(const char8_t* path);
    static bool Exists(const char8_t* path);
    static bool Move(const char8_t* srcPath, const char8_t* destPath);
    static bool SetCreationTime(const char8_t* path, const DateTime& creationTime);
    static bool SetCreationTimeUtc(const char8_t* path, const DateTime& creationTimeUtc);
    static bool SetLastAccessTime(const char8_t* path, const DateTime& lastAccessTime);
    static bool SetLastAccessTimeUtc(const char8_t* path, const DateTime& lastAccessTimeUtc);
    static bool SetLastWriteTime(const char8_t* path, const DateTime& lastWriteTime);
    static bool SetLastWriteTimeUtc(const char8_t* path, const DateTime& lastWriteTimeUtc);
    [[nodiscard]] static std::optional<DateTime> GetCreationTime(const char8_t* path);
    [[nodiscard]] static std::optional<DateTime> GetCreationTimeUtc(const char8_t* path);
    [[nodiscard]] static std::optional<DateTime> GetLastAccessTime(const char8_t* path);
    [[nodiscard]] static std::optional<DateTime> GetLastAccessTimeUtc(const char8_t* path);
    [[nodiscard]] static std::optional<DateTime> GetLastWriteTime(const char8_t* path);
    [[nodiscard]] static std::optional<DateTime> GetLastWriteTimeUtc(const char8_t* path);
    [[nodiscard]] static std::optional<FileAttributes> GetAttributes(const char8_t* path);
    static bool Decrypt(const char8_t* path);
    static bool Encrypt(const char8_t* path);
    [[nodiscard]] static std::optional<FileStream> Create(const char8_t* path);
    [[nodiscard]] static std::optional<FileStream> Create(const char8_t* path, int32_t bufferSize);
    [[nodiscard]] static std::optional<FileStream> Create(const char8_t* path, int32_t bufferSize, FileOptions options);
    [[nodiscard]] static std::optional<FileStream> Open(const char8_t* path, FileMode mode);
    [[nodiscard]] static std::optional<FileStream> Open(const char8_t* path, FileMode mode, FileAccess access);
    [[nodiscard]] static std::optional<FileStream> Open(const char8_t* path, FileMode mode, FileAccess access, FileShare share);
    static bool SetAttributes(const char8_t* path, FileAttributes fileAttributes);
    [[nodiscard]] static std::optional<std::u8string> ReadAllText(const char8_t* path);
    [[nodiscard]] static std::optional<std::u8string> ReadAllText(const char8_t* path, const Encoding& encoding);
    [[nodiscard]] static std::unique_ptr<std::byte[]> ReadAllBytes(const char8_t* path, ReturnPointerTag);
    [[nodiscard]] static std::optional<std::vector<std::byte>> ReadAllBytes(const char8_t* path, ReturnVectorTag);
    [[nodiscard]] static std::optional<std::vector<std::u8string>> ReadAllLines(const char8_t* path);
    template <typename _Predicate>
    static void ReadLines(const char8_t* path, const _Predicate& callback);
    //static void AppendAllLines(const std::u8string_view& path, IEnumerable<string> contents);
    //static void AppendAllLines(const std::u8string_view& path, IEnumerable<string> contents, Encoding encoding);
    //static void AppendAllText(const std::u8string_view& path, string contents);
    //static void AppendAllText(const std::u8string_view& path, string contents, Encoding encoding);
    //static StreamWriter AppendText(const std::u8string_view& path);
    //static StreamWriter CreateText(const std::u8string_view& path);
    //static FileStream OpenRead(const std::u8string_view& path);
    //static StreamReader OpenText(const std::u8string_view& path);
    //static FileStream OpenWrite(const std::u8string_view& path);
    //static string[] ReadAllLines(const std::u8string_view& path);
    //static string[] ReadAllLines(const std::u8string_view& path, Encoding encoding);
    //static IEnumerable<string> ReadLines(const std::u8string_view& path, Encoding encoding);
    //static void Replace(const std::u8string_view& sourcePath, string destinationPath, string destinationBackupPath);
    //static void Replace(const std::u8string_view& sourcePath, string destinationPath, string destinationBackupPath, bool ignoreMetadataErrors);
    static bool WriteAllBytes(const char8_t* path, const std::span<std::byte>& bytes);
    //static bool WriteAllLines(const char8_t* path, const std::span<std::u8string_view>& contents);
    //static void WriteAllLines(const std::u8string_view& path, string[] contents, Encoding encoding);
    //static void WriteAllLines(const std::u8string_view& path, IEnumerable<string> contents, Encoding encoding);
    //static void WriteAllText(const std::u8string_view& path, string contents);
    //static void WriteAllText(const std::u8string_view& path, string contents, Encoding encoding);
};

template <typename _Predicate>
void File::ReadLines(const char8_t* path, const _Predicate& callback)
{
//    std::basic_ifstream<char8_t, std::char_traits<char8_t>> fs;
//    fs.open(reinterpret_cast<const char*>(path));
//
//    if (!fs)
//    {
//        return;
//    }
//
//    std::u8string line;
//    while (std::getline(fs, line))
//    {
//        if (line.empty())
//        {
//            continue;
//        }
//        
//        if (callback(std::move(line)) == false)
//        {
//            break;
//        }
//    }
}

}
