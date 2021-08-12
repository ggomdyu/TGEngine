#pragma once

#include <cstring>
#ifdef _MSC_VER
#include <rpc.h>
#else
#include <uuid/uuid.h>
#endif

#include "FixedString.h"

#ifdef _MSC_VER
#pragma comment(lib, "Rpcrt4.lib")
#endif

namespace tg
{
//
//struct UUID :
//    private BasicFixedString<char, 37>
//{
///**@section Type */
//private:
//    using Super = BasicFixedString<char, 37>;
//
//public:
//    using ValueType = char;
//    using ConstReference = const char&;
//    using ConstIterator = const char*;
//    using ConstPointer = const char*;
//    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
//    
///**@section Constructor */
//private:
//    using Super::Super;
//
///**@section Operator */
//public:
//    using Super::operator=;
//    using Super::operator!=;
//    using Super::operator==;
//    using Super::operator std::basic_string<char>;
//    using Super::operator std::basic_string_view<char>;
//    const char operator[](int32_t index) const;
//    
///**@section Method */
//public:
//    static UUID NewUUID();
//    ConstIterator Begin() const noexcept;
//    ConstIterator End() const noexcept;
//    using Super::CompareTo;
//    using Super::IndexOf;
//    using Super::IndexOfAny;
//    using Super::LastIndexOf;
//    using Super::LastIndexOfAny;
//    using Super::Data;
//    using Super::Length;
//    using Super::Capacity;
//    using Super::CBegin;
//    using Super::CEnd;
//};
//
//inline UUID UUID::NewUUID()
//{
//#ifdef _MSC_VER
//    ::UUID rawUUID;
//    UuidCreate(&rawUUID);
//
//    RPC_CSTR rawUUIDStr;
//    UuidToStringA(&rawUUID, &rawUUIDStr);
//
//    UUID ret(reinterpret_cast<char*>(rawUUIDStr), 36);
//    RpcStringFreeA(&rawUUIDStr);
//#else
//    uuid_t uuid;
//    uuid_generate_random(uuid);
//
//    UUID ret;
//    ret.m_strLen = 36;
//    uuid_unparse(uuid, &ret.m_str[0]);
//#endif
//    return ret;
//}
//    
//inline UUID::ConstIterator UUID::Begin() const noexcept
//{
//    return this->CBegin();
//}
//
//inline UUID::ConstIterator UUID::End() const noexcept
//{
//    return this->CEnd();
//}
//   
//inline const char UUID::operator[](int32_t index) const
//{
//    return Super::operator[](index);
//}

}
