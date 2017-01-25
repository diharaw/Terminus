#pragma once

#include <stdint.h>
#include <deque>

template<typename DATA, size_t SIZE>
struct Deque
{
    DATA      _data[SIZE];
    uint32_t  _num_objects;
    uint32_t  _front;
    uint32_t  _back;
};

template<typename DATA, size_t SIZE>
struct HashMap
{
    struct HashEntry
    {
        uint64_t key;
        uint32_t next;
        DATA value;
    };
    
    uint32_t  _hash[SIZE];
    Deque<uint32_t, SIZE> _freelist;
    HashEntry _data[SIZE];
    uint32_t  _num_objects;
};
