#pragma once

TG_NAMESPACE_BEGIN

enum class FileMode
{
    CreateNew = 1,
    Create = 2,
    Open = 3,
    OpenOrCreate = 4,
    Truncate = 5,
    Append = 6,
};

TG_NAMESPACE_END
