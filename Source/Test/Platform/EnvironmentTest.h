/**
 * @file    EnvironmentTest.h
 * @author  ggomdyu
 * @since   08/22/2019
 */

#pragma once
#include <cassert>

#include "IO/Path.h"
#include "IO/File.h"
#include "Diagnostics/Debug.h"
#include "Platform/Environment.h"

#include "../Test.h"

namespace tgon
{

class EnvironmentTest :
    public Test
{
/**@section Method */
public:
    void Evaluate() override
    {
#if TGON_PLATFORM_MACOS
        assert(Environment::GetSystemPageSize() == 4096);
        assert(Environment::Is64BitProcess() == true);
        assert(Environment::Is64BitOperatingSystem() == true);
        assert(Environment::GetNewLine() == "\n");
#endif
        auto a = Environment::GetCommandLineArgs();
        auto b = Environment::GetMachineName();
        auto c = Environment::GetUserDomainName();
        auto d = Environment::GetUserName();
        auto e = Environment::GetEnvironmentVariable("PATH_TGON");
        assert(e.has_value() == false);
        auto f = Environment::SetEnvironmentVariable("PATH_TGON", u8"C:/HU7AI");
        auto g = Environment::GetEnvironmentVariable("PATH_TGON");
        assert(g.value() == u8"C:/HU7AI");
        auto h = Environment::GetCurrentDirectory();
        auto i = Environment::GetFolderPath(Environment::SpecialFolder::Desktop);
        auto j = Environment::GetCurrentManagedThreadId();
    }
};

} /* namespace tgon */
