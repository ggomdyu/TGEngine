#pragma once

TG_NAMESPACE_BEGIN

enum class FileAccess
{
    Read = 1,
    Write = 2,
    ReadWrite = Read | Write
};

TG_NAMESPACE_END
