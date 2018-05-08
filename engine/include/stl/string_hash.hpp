#pragma once

#include <core/terminus_macros.hpp>
#include <io/io_macros.hpp>
#include <stl/murmur_hash.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

struct StringHash
{

    uint64_t m_hash;
    
    StringHash()
    {
        m_hash = 0;
    }
    
    StringHash(const char* str)
    {
        m_hash = TE_HASH(str);
    }
    
    StringHash(uint64_t hash)
    {
        m_hash = hash;
    }
    
    bool operator== (const StringHash& other)
    {
        return m_hash == other.m_hash;
    }
    
    bool operator!= (const StringHash& other)
    {
        return m_hash != other.m_hash;
    }
    
    StringHash& operator= (const StringHash& other)
    {
        m_hash = other.m_hash;
        return *this;
    }
    
    StringHash& operator= (const char* str)
    {
        m_hash = TE_HASH(str);
        return *this;
    }
    
    StringHash& operator= (uint64_t hash)
    {
        m_hash = hash;
        return *this;
    }
};

TE_END_TERMINUS_NAMESPACE
