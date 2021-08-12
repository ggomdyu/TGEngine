#include "PrecompiledHeader.h"

#include <array>
#include <thread>

#include "IO/Directory.h"

#include "Environment.h"

namespace tg
{

thread_local std::array<char8_t, 32768> g_tempUtf8StrBuffer;

std::optional<std::u8string> Environment::GetEnvironmentVariable(const char8_t* name)
{
    const auto strLen = GetEnvironmentVariable(name, g_tempUtf8StrBuffer.data(), static_cast<int32_t>(g_tempUtf8StrBuffer.size()));
    if (strLen.has_value() == false)
    {
        return {};
    }

    return std::u8string(g_tempUtf8StrBuffer.data(), static_cast<size_t>(*strLen));
}

std::optional<int32_t> Environment::GetEnvironmentVariable(const char8_t* name, const std::span<char8_t>& destStr)
{
    return GetEnvironmentVariable(name, &destStr[0], static_cast<int32_t>(destStr.size()));
}

std::optional<int32_t> Environment::GetEnvironmentVariable(const char8_t* name, EnvironmentVariableTarget target, const std::span<char8_t>& destStr)
{
    return GetEnvironmentVariable(name, target, destStr.data(), static_cast<int32_t>(destStr.size()));
}

std::optional<std::u8string> Environment::GetEnvironmentVariable(const char8_t* name, EnvironmentVariableTarget target)
{
    const auto strLen = GetEnvironmentVariable(name, target, g_tempUtf8StrBuffer.data(), static_cast<int32_t>(g_tempUtf8StrBuffer.size()));
    if (strLen.has_value() == false)
    {
        return {};
    }

    return std::u8string(g_tempUtf8StrBuffer.data(), static_cast<size_t>(*strLen));
}

std::u8string Environment::GetCurrentDirectory()
{
    return Directory::GetCurrentDirectory();
}

std::optional<int32_t> Environment::GetCurrentDirectory(char8_t* destStr, int32_t destStrBufferLen)
{
    return Directory::GetCurrentDirectory(destStr, destStrBufferLen);
}

std::optional<int32_t> Environment::GetCurrentDirectory(const std::span<char8_t>& destStr)
{
    return GetCurrentDirectory(destStr.data(), static_cast<int32_t>(destStr.size()));
}

std::u8string Environment::GetFolderPath(SpecialFolder folder)
{
    const auto strLen = GetFolderPath(folder, g_tempUtf8StrBuffer.data(), static_cast<int32_t>(g_tempUtf8StrBuffer.size()));
    return {g_tempUtf8StrBuffer.data(), static_cast<size_t>(*strLen)};
}

void Environment::Exit(int32_t exitCode)
{
    std::exit(exitCode);
}

int32_t Environment::GetProcessorCount()
{
    return static_cast<int32_t>(std::thread::hardware_concurrency());
}

std::u8string Environment::GetUserName()
{
    const auto strLen = GetUserName(g_tempUtf8StrBuffer.data(), static_cast<int32_t>(g_tempUtf8StrBuffer.size()));
    return {g_tempUtf8StrBuffer.data(), static_cast<size_t>(*strLen)};
}

std::u8string Environment::GetMachineName()
{
    auto strLen = GetMachineName(g_tempUtf8StrBuffer.data(), static_cast<int32_t>(g_tempUtf8StrBuffer.size()));
    return {g_tempUtf8StrBuffer.data(), static_cast<size_t>(*strLen)};
}

std::u8string Environment::GetUserDomainName()
{
    auto strLen = GetUserDomainName(g_tempUtf8StrBuffer.data(), static_cast<int32_t>(g_tempUtf8StrBuffer.size()));
    return {g_tempUtf8StrBuffer.data(), static_cast<size_t>(*strLen)};
}

std::u8string Environment::GetStackTrace()
{
    const auto strLen = GetStackTrace(g_tempUtf8StrBuffer.data(), static_cast<int32_t>(g_tempUtf8StrBuffer.size()));
    return {g_tempUtf8StrBuffer.data(), static_cast<size_t>(strLen)};
}

std::u8string Environment::GetSystemDirectory()
{
    return GetFolderPath(SpecialFolder::System);
}

std::vector<std::u8string> Environment::GetLogicalDrives()
{
    return Directory::GetLogicalDrives();
}

}
