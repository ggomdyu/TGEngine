#include "PrecompiledHeader.h"

#include <cassert>
#include <cmath>
#include <string>
#include <map>

#include "Hash.h"

namespace tg
{
    
//class HashCollisionChecker
//{
//public:
//    
//private:
//    std::map<uint32_t, std::string> m_hashStringTable;
//}

//    class HashCollisionChecker
//    {
//    public:
//
//
//    private:
//        std::map<HashValue, std::string> m_hashCollisionCheck;
//    };

//inline std::size_t Hash::x65599Hash(const char* str, std::size_t length)
//{
//    std::size_t hash = 0;
//    for (std::size_t i = 0; i < length; ++i)
//    {
//        hash = 65599 * hash + str[i];
//    }
//
//    hash = hash ^ (hash >> 16);
//
//#if defined(_DEBUG) || defined(DEBUG)
//    struct hash_x65599 {};
//
//    if (Hash::CheckHashCollision<hash_x65599>(hash, str))
//    {
//        // TODO: How about USE logger?
//        assert(false && "DANGEROUS WARNING! :: Hash collision occured!!!");
//    }
//#endif
//
//    return hash;
//}
//
//template <typename HashAlgorithmTag>
//inline bool Hash::CheckHashCollision(std::size_t hash, const std::string& str)
//{
//    static std::map<std::size_t, std::string> hashCollisionChecker;
//
//    auto iter = hashCollisionChecker.find(hash);
//    if (iter != hashCollisionChecker.end())
//    {
//        if (std::strcmp(iter->second.c_str(), str.c_str()))
//        {
//            return true;
//        }
//    }
//    else
//    {
//        hashCollisionChecker.insert({ hash, str });
//    }
//    
//    return false;
//}
//

}
