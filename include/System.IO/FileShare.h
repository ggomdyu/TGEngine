#pragma once

TG_NAMESPACE_BEGIN

enum class FileShare
{
    None = 0,
    Read = 1,
    Write = 2,
    ReadWrite = 3,
    Delete = 4,
    Inheritable = 16
};

TG_NAMESPACE_END
