#pragma once

#include <fmt/format.h>

#include "Diagnostics/Debug.h"

#if defined(_DEBUG) || !defined(NDEBUG)
#   define TGON_AL_ERROR_CHECK(expression)\
    expression;\
    {\
        auto errorCode = alGetError();\
        if (errorCode != AL_NO_ERROR)\
        {\
            tg::Debug::WriteLine(fmt::format(u8"Failed to invoke AL Function. (Code:{0}, Expression:{1})", errorCode, reinterpret_cast<const char8_t*>(#expression)));\
        }\
    }
#else
#   define TGON_AL_ERROR_CHECK(expression) expression;
#endif