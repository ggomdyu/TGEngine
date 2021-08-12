#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

#undef CreateDirectory
#undef CreateFont
#undef SetCurrentDirectory
#undef SetEnvironmentVariable
#undef GetCurrentDirectory
#undef GetEnvironmentVariable
#undef GetCommandLine
#undef GetSystemDirectory
#undef GetUserName
#undef GetTempPath