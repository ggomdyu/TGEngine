#include "PrecompiledHeader.h"

#include <array>
#include <Platform/Windows/Windows.h>
#ifndef SECURITY_WIN32
#define SECURITY_WIN32
#endif
#include <Psapi.h>
#include <security.h>
#include <shlobj.h>
#include <sstream>
#pragma pack(push, before_imagehlp, 8)
#include <imagehlp.h>
#pragma pack(pop, before_imagehlp)

#include "SafeHandle.h"
#include "SafeRegistryHandle.h"

#include "../Environment.h"
#include "../OperatingSystem.h"

#pragma comment(lib, "Secur32.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "dbghelp.lib")

#define TGON_THROW_SEH_EXCEPTION() volatile int temp = 0; temp = 1 / temp;

namespace tg
{

extern thread_local std::array<char8_t, 32768> g_tempUtf8StrBuffer;
thread_local std::array<wchar_t, 32768> g_tempUtf16StrBuffer;

namespace
{

constexpr int MaxEnvVariableValueLength = 32767;
constexpr auto MaxSystemEnvVariableLength = 1024;
constexpr int MaxUserEnvVariableLength = 255;

[[nodiscard]] bool CheckEnvironmentVariableName(const std::u8string_view& name) noexcept
{
    if (name.length() == 0)
    {
        return false;
    }

    if (name[0] == '\0')
    {
        return false;
    }

    if (name.length() >= MaxEnvVariableValueLength)
    {
        return false;
    }

    if (name.find(u8'=') != std::u8string_view::npos)
    {
        return false;
    }

    return true;
}

[[nodiscard]] std::optional<std::wstring> Utf8ToUtf16(const std::u8string_view& str)
{
    const auto utf16StrLen = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(str.data()), static_cast<int>(str.length()) + 1, nullptr, 0) - 1;
    if (utf16StrLen == -1)
    {
        return {};
    }

    std::wstring utf16Str;
    utf16Str.resize(utf16StrLen);
    MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(str.data()), static_cast<int>(str.length()) + 1, utf16Str.data(), static_cast<int>(utf16Str.size()) == 0);

    return std::move(utf16Str);
};

[[nodiscard]] std::optional<std::u8string> Utf16ToUtf8(const std::wstring_view& str)
{
    const auto utf8StrLen = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.length()) + 1, nullptr, 0, nullptr, nullptr) - 1;
    if (utf8StrLen == -1)
    {
        return {};
    }

    std::u8string utf8Str;
    utf8Str.resize(utf8StrLen);
    WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.length()) + 1, reinterpret_cast<char*>(utf8Str.data()), static_cast<int>(utf8Str.size()), nullptr, nullptr);

    return std::move(utf8Str);
};

bool InternalSetEnvironmentVariable(HKEY predefinedKey, LPCWSTR subKey, const std::u8string_view& name, const char8_t* value)
{
    SafeRegistryHandle keyHandle;
    if (RegOpenKeyExW(predefinedKey, subKey, 0, KEY_READ | KEY_WRITE, keyHandle) != ERROR_SUCCESS)
    {
        return false;
    }

    if (MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(name.data()), static_cast<int>(name.length()) + 1, g_tempUtf16StrBuffer.data(), g_tempUtf16StrBuffer.size()) == 0)
    {
        return {};
    }

    if (value == nullptr)
    {
        RegDeleteValueW(keyHandle, g_tempUtf16StrBuffer.data());
    }
    else if (RegSetValueExW(keyHandle, g_tempUtf16StrBuffer.data(), 0, REG_SZ, reinterpret_cast<const BYTE*>(value), std::char_traits<char8_t>::length(value) + 1) != ERROR_SUCCESS)
    {
        return false;
    }

    return true;
}

std::optional<int32_t> InternalGetEnvironmentVariable(HKEY predefinedKey, LPCWSTR subKey, const std::u8string_view& name, char8_t* destStr, int32_t destStrBufferLen)
{
    SafeRegistryHandle keyHandle;
    if (RegOpenKeyExW(predefinedKey, subKey, 0, KEY_READ, keyHandle) != ERROR_SUCCESS)
    {
        return {};
    }

    auto& utf16Name = g_tempUtf16StrBuffer;
    if (MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(name.data()), static_cast<int>(name.length()) + 1, utf16Name.data(), utf16Name.size()) == 0)
    {
        return {};
    }

    DWORD type = 0;
    DWORD valueByteCount = destStrBufferLen;
    if (RegQueryValueExW(keyHandle, utf16Name.data(), nullptr, &type, reinterpret_cast<LPBYTE>(destStr), reinterpret_cast<LPDWORD>(&valueByteCount)) != ERROR_SUCCESS)
    {
        return {};
    }

    return valueByteCount - 1;
}

}

bool Environment::SetEnvironmentVariable(const char8_t* name, const char8_t* value)
{
    auto utf16Name = Utf8ToUtf16(name);
    if (utf16Name.has_value() == false)
    {
        return false;
    }

    auto utf16Value = Utf8ToUtf16(value);
    if (utf16Value.has_value() == false)
    {
        return false;
    }

    return !!SetEnvironmentVariableW(utf16Name->data(), utf16Value->data());
}

bool Environment::SetEnvironmentVariable(const char8_t* name, const char8_t* value, EnvironmentVariableTarget target)
{
    if (target == EnvironmentVariableTarget::Process)
    {
        return SetEnvironmentVariable(name, value);
    }

    const auto nameLen = std::char_traits<char8_t>::length(name);
    if (CheckEnvironmentVariableName({name, nameLen}) == false)
    {
        return false;
    }

    if (target == EnvironmentVariableTarget::Machine)
    {
        if (nameLen >= MaxSystemEnvVariableLength)
        {
            return false;
        }

        return InternalSetEnvironmentVariable(HKEY_LOCAL_MACHINE, L"System\\CurrentControlSet\\Control\\Session Manager\\Environment", {name, nameLen}, value);
    }

    if (nameLen >= MaxUserEnvVariableLength)
    {
        return false;
    }

    return InternalSetEnvironmentVariable(HKEY_CURRENT_USER, L"Environment", {name, nameLen}, value);
}

std::optional<int32_t> Environment::GetEnvironmentVariable(const char8_t* name, char8_t* destStr, int32_t destStrBufferLen)
{
    std::array<wchar_t, 2048> utf16Name{};
    if (MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(name), -1, &utf16Name[0], static_cast<DWORD>(utf16Name.size())) == 0)
    {
        return {};
    }

    std::array<wchar_t, 2048> utf16Value{};
    if (GetEnvironmentVariableW(&utf16Name[0], &utf16Value[0], static_cast<DWORD>(utf16Value.size())) == 0)
    {
        return {};
    }

    const auto utf8ValueLen = WideCharToMultiByte(CP_UTF8, 0, &utf16Value[0], -1, reinterpret_cast<char*>(destStr), destStrBufferLen, nullptr, nullptr) - 1;
    if (utf8ValueLen == -1)
    {
        return {};
    }

    return utf8ValueLen;
}

std::optional<int32_t> Environment::GetEnvironmentVariable(const char8_t* name, EnvironmentVariableTarget target, char8_t* destStr, int32_t destStrBufferLen)
{
    if (target == EnvironmentVariableTarget::Process)
    {
        return GetEnvironmentVariable(name, destStr, destStrBufferLen);
    }

    if (target == EnvironmentVariableTarget::Machine)
    {
        return InternalGetEnvironmentVariable(HKEY_LOCAL_MACHINE, L"System\\CurrentControlSet\\Control\\Session Manager\\Environment", name, destStr, destStrBufferLen);
    }

    return InternalGetEnvironmentVariable(HKEY_CURRENT_USER, L"Environment", name, destStr, destStrBufferLen);
}

std::optional<int32_t> Environment::GetFolderPath(SpecialFolder folder, char8_t* destStr, int32_t destStrBufferLen)
{
    if (SHGetFolderPathW(nullptr, static_cast<int>(folder), nullptr, 0, g_tempUtf16StrBuffer.data()) == S_OK)
    {
        const auto utf16FolderPathLen = WideCharToMultiByte(CP_UTF8, 0, g_tempUtf16StrBuffer.data(), -1, reinterpret_cast<char*>(destStr), destStrBufferLen, nullptr, nullptr) - 1;
        if (utf16FolderPathLen >= 0)
        {
            return utf16FolderPathLen;
        }
    }

    return {};
}

std::optional<int32_t> Environment::GetFolderPath(SpecialFolder folder, const std::span<char8_t>& destStr)
{
    return GetFolderPath(folder, &destStr[0], static_cast<int32_t>(destStr.size()));
}

const std::u8string& Environment::GetCommandLine()
{
    static auto commandLine = []()
    {
        const wchar_t* utf16CommandLine = GetCommandLineW();

        const auto utf16CommandLineByteCount = WideCharToMultiByte(CP_UTF8, 0, utf16CommandLine, -1, nullptr, 0, nullptr, nullptr);
        if (utf16CommandLineByteCount == 0)
        {
            return std::u8string();
        }

        std::u8string utf8CommandLine;
        utf8CommandLine.resize(static_cast<size_t>(utf16CommandLineByteCount) - 1);
        if (WideCharToMultiByte(CP_UTF8, 0, utf16CommandLine, -1, reinterpret_cast<char*>(utf8CommandLine.data()), static_cast<int>(utf8CommandLine.length()) + 1, nullptr, nullptr) == 0)
        {
            return std::u8string();
        }

        return utf8CommandLine;
    } ();
    return commandLine;
}

const std::vector<std::u8string>& Environment::GetCommandLineArgs()
{
    static auto commandLineArgs = []()
    {
        std::vector<std::u8string> ret;
        
        std::basic_stringstream<char8_t, std::char_traits<char8_t>, std::allocator<char8_t>> ss(GetCommandLine());
        std::u8string commandLineArg;
        while (std::getline(ss, commandLineArg, u8' '))
        {
            ret.push_back(std::move(commandLineArg));
        }
        
        return ret;
    } ();
    
    return commandLineArgs;
}

int64_t Environment::GetTickCount()
{
    return static_cast<int64_t>(::GetTickCount64());
}

bool Environment::Is64BitProcess()
{
#if defined(_WIN64)
    return true;
#else
    return false;
#endif
}

bool Environment::Is64BitOperatingSystem()
{
#if defined(_WIN64)
    // 64-bit programs run only on 64-bit 
    return true;
#else
    BOOL isWow64 = FALSE;
    return IsWow64Process(GetCurrentProcess(), &isWow64) && isWow64;
#endif
}

void Environment::FailFast(const char8_t* message)
{
    const auto utf16MessageLen = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(message), -1, nullptr, 0) - 1;
    if (utf16MessageLen != -1)
    {
        std::wstring utf16Message;
        utf16Message.resize(static_cast<size_t>(utf16MessageLen));
        MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(message), -1, &utf16Message[0], utf16MessageLen);

        OutputDebugStringW(L"FailFast:\n");
        OutputDebugStringW(&utf16Message[0]);
    }

    assert(false);
}

std::u8string_view Environment::GetNewLine()
{
    char8_t newLine[] = u8"\r\n";
    return {newLine, std::extent_v<decltype(newLine)> - 1};
}

int32_t Environment::GetSystemPageSize()
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    return static_cast<int32_t>(si.dwPageSize);
}

int32_t Environment::GetCurrentManagedThreadId()
{
    return GetCurrentThreadId();
}

std::optional<int32_t> Environment::GetUserName(char8_t* destStr, int32_t destStrBufferLen)
{
    auto utf16UserNameBufferLen = static_cast<DWORD>(g_tempUtf16StrBuffer.size());
    if (GetUserNameW(g_tempUtf16StrBuffer.data(), & utf16UserNameBufferLen) == FALSE)
    {
        return {};
    }

    const auto utf8UserNameLen = WideCharToMultiByte(CP_UTF8, 0, g_tempUtf16StrBuffer.data(), -1, reinterpret_cast<char*>(destStr), destStrBufferLen, nullptr, nullptr) - 1;
    if (utf8UserNameLen == -1)
    {
        return {};
    }

    return utf8UserNameLen;
}

std::optional<int32_t> Environment::GetUserName(const std::span<char8_t>& destStr)
{
    return GetUserName(&destStr[0], static_cast<int32_t>(destStr.size()));
}

std::optional<int32_t> Environment::GetMachineName(char8_t* destStr, int32_t destStrBufferLen)
{
    auto utf16ComputerNameBufferLen = static_cast<DWORD>(g_tempUtf16StrBuffer.size());
    if (GetComputerNameW(g_tempUtf16StrBuffer.data(), &utf16ComputerNameBufferLen) == FALSE)
    {
        return {};
    }

    const auto utf8ComputerNameLen = WideCharToMultiByte(CP_UTF8, 0, g_tempUtf16StrBuffer.data(), -1, reinterpret_cast<char*>(destStr), destStrBufferLen, nullptr, nullptr) - 1;
    if (utf8ComputerNameLen == -1)
    {
        return {};
    }

    return utf8ComputerNameLen;
}

std::optional<int32_t> Environment::GetMachineName(const std::span<char8_t>& destStr)
{
    return GetMachineName(&destStr[0], static_cast<int32_t>(destStr.size()));
}

std::optional<int32_t> Environment::GetUserDomainName(char8_t* destStr, int32_t destStrBufferLen)
{
    auto utf16UserNameBufferLen = static_cast<DWORD>(g_tempUtf16StrBuffer.size());
    if (GetUserNameExW(NameSamCompatible, g_tempUtf16StrBuffer.data(), &utf16UserNameBufferLen) == TRUE)
    {
        if (const wchar_t* str = wcschr(g_tempUtf16StrBuffer.data(), L'\\'); str != nullptr)
        {
            const auto utf8UserNameLen = WideCharToMultiByte(CP_UTF8, 0, g_tempUtf16StrBuffer.data(), str - g_tempUtf16StrBuffer.data(), reinterpret_cast<char*>(destStr), destStrBufferLen, nullptr, nullptr);
            if (utf8UserNameLen == 0)
            {
                return {};
            }

            destStr[utf8UserNameLen] = u8'\0';
            return utf8UserNameLen;
        }
    }

    utf16UserNameBufferLen = static_cast<DWORD>(g_tempUtf16StrBuffer.size());

    if (GetUserNameW(g_tempUtf16StrBuffer.data(), &utf16UserNameBufferLen) == TRUE)
    {
        SID_NAME_USE peUse;
        std::array<wchar_t, 1024> sid{};
        auto sidLen = static_cast<DWORD>(sid.size());
        std::array<wchar_t, 2048> utf16AccountName{};
        auto utf16AccountNameBufferLen = static_cast<DWORD>(g_tempUtf16StrBuffer.size());
        if (LookupAccountNameW(nullptr, g_tempUtf16StrBuffer.data(), &sid[0], &sidLen, &utf16AccountName[0], &utf16AccountNameBufferLen, &peUse) == TRUE)
        {
            const auto utf8UserNameLen = WideCharToMultiByte(CP_UTF8, 0, &utf16AccountName[0], -1, reinterpret_cast<char*>(destStr), destStrBufferLen, nullptr, nullptr) - 1;
            if (utf8UserNameLen == -1)
            {
                return {};
            }

            destStr[utf8UserNameLen] = u8'\0';
            return utf8UserNameLen;
        }
    }

    return {};
}

std::optional<int32_t> Environment::GetUserDomainName(const std::span<char8_t>& destStr)
{
    return GetUserDomainName(&destStr[0], static_cast<int32_t>(destStr.size()));
}

DWORD InternalGetStackTrace(EXCEPTION_POINTERS* ep, char8_t* destStr, int32_t destStrBufferLen, int32_t* destStrLen)
{
    auto* threadHandle = GetCurrentThread();
    auto* processHandle = GetCurrentProcess();
    if (SymInitialize(processHandle, nullptr, false) == FALSE)
    {
        *destStrLen = 0;
        return EXCEPTION_EXECUTE_HANDLER;
    }

    SymSetOptions(SymGetOptions() | SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
    
    CONTEXT* context = ep->ContextRecord;
#ifdef _M_X64
    STACKFRAME64 frame
    {
        .AddrPC = ADDRESS64{
            .Offset = context->Rip,
            .Mode = AddrModeFlat
        },
        .AddrFrame = ADDRESS64{
            .Offset = context->Rbp,
            .Mode = AddrModeFlat
        },
        .AddrStack = ADDRESS64{
            .Offset = context->Rsp,
            .Mode = AddrModeFlat
        },
    };
#else
    STACKFRAME64 frame
    {
        .AddrPC = ADDRESS64{
            .Offset = context->Eip,
            .Mode = AddrModeFlat
        },
        .AddrFrame = ADDRESS64{
            .Offset = context->Ebp,
            .Mode = AddrModeFlat
        },
        .AddrStack = ADDRESS64{
            .Offset = context->Esp,
            .Mode = AddrModeFlat
        },
    };
#endif
    
    std::vector<HMODULE> moduleHandles;
    DWORD moduleHandlesByteCount = 0;
    EnumProcessModules(processHandle, nullptr, 0, &moduleHandlesByteCount);
    moduleHandles.resize(moduleHandlesByteCount / sizeof(HMODULE));
    EnumProcessModules(processHandle, &moduleHandles[0], static_cast<DWORD>(moduleHandles.size() * sizeof(HMODULE)), &moduleHandlesByteCount);

    void* baseModuleAddress = nullptr;
    for (auto it = moduleHandles.rbegin(); it != moduleHandles.rend(); ++it)
    {
        const HMODULE moduleHandle = *it;
        MODULEINFO moduleInfo;
        GetModuleInformation(processHandle, moduleHandle, &moduleInfo, sizeof(moduleInfo));

        std::array<char8_t, 4096> imageName{};
        GetModuleFileNameExA(processHandle, moduleHandle, reinterpret_cast<char*>(&imageName[0]), sizeof(imageName));
        std::array<char8_t, 4096> moduleName{};
        GetModuleBaseNameA(processHandle, moduleHandle, reinterpret_cast<char*>(&moduleName[0]), sizeof(moduleName));

        SymLoadModule64(processHandle, 0, reinterpret_cast<char*>(&imageName[0]), reinterpret_cast<char*>(&moduleName[0]), reinterpret_cast<DWORD64>(moduleInfo.lpBaseOfDll), moduleInfo.SizeOfImage);

        baseModuleAddress = moduleInfo.lpBaseOfDll;
    }

    const PIMAGE_NT_HEADERS image = ImageNtHeader(baseModuleAddress);
    const DWORD imageType = image->FileHeader.Machine;

    IMAGEHLP_LINE64 line{};
    line.SizeOfStruct = sizeof(line);

    DWORD64 displacement = 0;
    DWORD offsetFromSymbol = 0;
    std::array<char8_t, 1024> undecoratedName{};

    *destStrLen = 0;

    do
    {
        if (frame.AddrPC.Offset != 0)
        {
            std::array<std::byte, sizeof(IMAGEHLP_SYMBOL64) + 1024> symBytes{};
            auto* sym = reinterpret_cast<IMAGEHLP_SYMBOL64*>(&symBytes[0]);
            sym->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
            sym->MaxNameLength = static_cast<DWORD>(undecoratedName.size());
            SymGetSymFromAddr64(processHandle, frame.AddrPC.Offset, &displacement, sym);

            // Get the undecorated name of function
            if (UnDecorateSymbolName(sym->Name, reinterpret_cast<char*>(&undecoratedName[0]), sym->MaxNameLength, UNDNAME_COMPLETE) == FALSE)
            {
                continue;
            }

            // Get the file name and line
            SymGetLineFromAddr64(processHandle, frame.AddrPC.Offset, &offsetFromSymbol, &line);

            *destStrLen += sprintf_s(reinterpret_cast<char*>(destStr) + *destStrLen, static_cast<size_t>(destStrBufferLen) - *destStrLen, "   at %s in %s:line %d\n", reinterpret_cast<const char*>(&undecoratedName[0]), line.FileName, line.LineNumber);
        }

        if (StackWalk64(imageType, processHandle, threadHandle, &frame, context, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr) == FALSE)
        {
            break;
        }
    } while (frame.AddrReturn.Offset != 0);

    SymCleanup(processHandle);

    return EXCEPTION_EXECUTE_HANDLER;
}

int32_t Environment::GetStackTrace(char8_t* destStr, int32_t destStrBufferLen)
{
    int32_t destStrLen = 0;
    __try
    {
        TGON_THROW_SEH_EXCEPTION();
    }
    __except (InternalGetStackTrace(GetExceptionInformation(), destStr, destStrBufferLen, &destStrLen))
    {
    }

    return destStrLen;
}

std::map<std::u8string, std::u8string> Environment::GetEnvironmentVariables()
{
    const std::unique_ptr<WCHAR[], void(*)(LPWCH)> strings(GetEnvironmentStringsW(), [](LPWCH strings)
    {
        FreeEnvironmentStringsW(strings);
    });
    std::map<std::u8string, std::u8string> ret;

    for (size_t i = 0;; ++i)
    {
        if (strings[i] == L'\0' && strings[i + 1] == L'\0')
        {
            break;
        }

        const auto startKey = i;
        while (strings[i] != '=' && strings[i] != '\0')
        {
            ++i;
        }

        if (strings[i] == '\0')
        {
            continue;
        }

        if (i - startKey == 0)
        {
            while (strings[i] != 0)
            {
                i++;
            }
            continue;
        }

        auto key = Utf16ToUtf8({&strings[startKey], i - startKey});
        if (key.has_value() == false)
        {
            continue;
        }

        const auto startValue = ++i;
        while (strings[i] != L'\0')
        {
            ++i;
        }

        auto value = Utf16ToUtf8({&strings[startValue], i - startValue});
        if (value.has_value() == false)
        {
            continue;
        }

        ret[std::move(*key)] = std::move(*value);
    }

    return ret;
}

std::optional<OperatingSystem> Environment::GetOSVersion()
{
    OSVERSIONINFOEX osvi
    {
        .dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX)
    };
    if (GetVersionExW(reinterpret_cast<OSVERSIONINFO*>(&osvi)) == FALSE)
    {
        return {};
    }

    const auto csdVersionLen = WideCharToMultiByte(CP_UTF8, 0, &osvi.szCSDVersion[0], -1, reinterpret_cast<char*>(g_tempUtf8StrBuffer.data()), g_tempUtf8StrBuffer.size(), nullptr, nullptr) - 1;
    if (csdVersionLen == -1)
    {
        return {};
    }

    const Version version(osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber, (osvi.wServicePackMajor << 16) | osvi.wServicePackMinor);
    return OperatingSystem(PlatformID::Win32NT, version, std::u8string(g_tempUtf8StrBuffer.data(), static_cast<size_t>(csdVersionLen)));
}

bool Environment::GetUserInteractive()
{
    static HWINSTA cachedWindowStationHandle = nullptr;
    static bool isUserNonInteractive = false;

    auto* const windowStationHandle = GetProcessWindowStation();
    if (windowStationHandle != nullptr && windowStationHandle != cachedWindowStationHandle)
    {
        USEROBJECTFLAGS flags;
        DWORD needLength;
        if (GetUserObjectInformationW(windowStationHandle, UOI_FLAGS, reinterpret_cast<PVOID>(&flags), sizeof(flags), &needLength) == TRUE)
        {
            if ((flags.dwFlags & WSF_VISIBLE) == 0)
            {
                isUserNonInteractive = true;
            }
        }

        cachedWindowStationHandle = windowStationHandle;
    }

    return !isUserNonInteractive;
}

int64_t Environment::GetWorkingSet()
{
    PROCESS_MEMORY_COUNTERS_EX pmc;
    const SafeHandle processHandle(OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId()));

    if (GetProcessMemoryInfo(processHandle, reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc)) == TRUE)
    {
        return pmc.WorkingSetSize;
    }

    return 0;
}

}

#undef TGON_THROW_SEH_EXCEPTION