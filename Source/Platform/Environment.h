#pragma once

#include <map>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "OperatingSystem.h"

namespace tg
{

enum class EnvironmentVariableTarget
{
    Process = 0,
    User = 1,
    Machine = 2,
};

class Environment final
{
/**@section Constructor */
public:
    Environment() = delete;

/**@section Enum */
public:
    enum class SpecialFolder
    {
        Desktop = 0,
        Programs = 2,
        MyDocuments = 5,
        Personal = 5,
        Favorites = 6,
        Startup = 7,
        Recent = 8,
        SendTo = 9,
        StartMenu = 11,
        MyMusic = 13,
        MyVideos = 14,
        DesktopDirectory = 16,
        MyComputer = 17,
        NetworkShortcuts = 19,
        Fonts = 20,
        Templates = 21,
        CommonStartMenu = 22,
        CommonPrograms = 23,
        CommonStartup = 24,
        CommonDesktopDirectory = 25,
        ApplicationData = 26,
        PrinterShortcuts = 27,
        LocalApplicationData = 28,
        InternetCache = 32,
        Cookies = 33,
        History = 34,
        CommonApplicationData = 35,
        Windows = 36,
        System = 37,
        ProgramFiles = 38,
        MyPictures = 39,
        UserProfile = 40,
        SystemX86 = 41,
        ProgramFilesX86 = 42,
        CommonProgramFiles = 43,
        CommonProgramFilesX86 = 44,
        CommonTemplates = 45,
        CommonDocuments = 46,
        CommonAdminTools = 47,
        AdminTools = 48,
        CommonMusic = 53,
        CommonPictures = 54,
        CommonVideos = 55,
        Resources = 56,
        LocalizedResources = 57,
        CommonOemLinks = 58,
        CDBurning = 59,
    };

/**@section Method */
public:
    static bool SetEnvironmentVariable(const char8_t* name, const char8_t* value);
    static bool SetEnvironmentVariable(const char8_t* name, const char8_t* value, EnvironmentVariableTarget target);
    static std::optional<int32_t> GetEnvironmentVariable(const char8_t* name, char8_t* destStr, int32_t destStrBufferLen);
    static std::optional<int32_t> GetEnvironmentVariable(const char8_t* name, const std::span<char8_t>& destStr);
    [[nodiscard]] static std::optional<std::u8string> GetEnvironmentVariable(const char8_t* name);
    static std::optional<int32_t> GetEnvironmentVariable(const char8_t* name, EnvironmentVariableTarget target, char8_t* destStr, int32_t destStrBufferLen);
    static std::optional<int32_t> GetEnvironmentVariable(const char8_t* name, EnvironmentVariableTarget target, const std::span<char8_t>& destStr);
    [[nodiscard]] static std::optional<std::u8string> GetEnvironmentVariable(const char8_t* name, EnvironmentVariableTarget target);
    static std::optional<int32_t> GetCurrentDirectory(char8_t* destStr, int32_t destStrBufferLen);
    static std::optional<int32_t> GetCurrentDirectory(const std::span<char8_t>& destStr);
    [[nodiscard]] static std::u8string GetCurrentDirectory();
    static std::optional<int32_t> GetFolderPath(SpecialFolder folder, char8_t* destStr, int32_t destStrBufferLen);
    static std::optional<int32_t> GetFolderPath(SpecialFolder folder, const std::span<char8_t>& destStr);
    [[nodiscard]] static std::u8string GetFolderPath(SpecialFolder folder);
    [[nodiscard]] static std::u8string_view GetNewLine();
    [[nodiscard]] static int32_t GetSystemPageSize();
    [[nodiscard]] static int32_t GetCurrentManagedThreadId();
    [[nodiscard]] static std::u8string GetUserName();
    [[nodiscard]] static std::optional<int32_t> GetUserName(char8_t* destStr, int32_t destStrBufferLen);
    [[nodiscard]] static std::optional<int32_t> GetUserName(const std::span<char8_t>& destStr);
    [[nodiscard]] static std::u8string GetMachineName();
    [[nodiscard]] static std::optional<int32_t> GetMachineName(char8_t* destStr, int32_t destStrBufferLen);
    [[nodiscard]] static std::optional<int32_t> GetMachineName(const std::span<char8_t>& destStr);
    [[nodiscard]] static std::u8string GetUserDomainName();
    [[nodiscard]] static std::optional<int32_t> GetUserDomainName(char8_t* destStr, int32_t destStrBufferLen);
    [[nodiscard]] static std::optional<int32_t> GetUserDomainName(const std::span<char8_t>& destStr);
    [[nodiscard]] static const std::u8string& GetCommandLine();
    [[nodiscard]] static const std::vector<std::u8string>& GetCommandLineArgs();
    [[noreturn]] static void Exit(int32_t exitCode);
    [[nodiscard]] static int64_t GetTickCount();
    [[nodiscard]] static int32_t GetProcessorCount();
    [[nodiscard]] static bool Is64BitProcess();
    [[nodiscard]] static bool Is64BitOperatingSystem();
    [[noreturn]] static void FailFast(const char8_t* message);
    static int32_t GetStackTrace(char8_t* destStr, int32_t destStrBufferLen);
    [[nodiscard]] static std::u8string GetStackTrace();
    [[nodiscard]] static std::u8string GetSystemDirectory();
    [[nodiscard]] static std::map<std::u8string, std::u8string> GetEnvironmentVariables();
    //[[nodiscard]] static std::map<std::u8string, std::u8string> GetEnvironmentVariables(EnvironmentVariableTarget target);
    //public static string ExpandEnvironmentVariables(string name);
    [[nodiscard]] static std::optional<OperatingSystem> GetOSVersion();
    [[nodiscard]] static bool GetUserInteractive();
    [[nodiscard]] static int64_t GetWorkingSet();
    [[nodiscard]] static std::vector<std::u8string> GetLogicalDrives();
};

}