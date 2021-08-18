#pragma once

TG_NAMESPACE_BEGIN

enum class SocketType
{
    Unknown = -1,
    Stream = 1,
    Dgram = 2,
    Raw = 3,
    Rdm = 4,
    Seqpacket = 5,
};

TG_NAMESPACE_END