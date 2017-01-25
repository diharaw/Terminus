#pragma once 

#include <Utility/murmur_hash.h>
#include <container/container_types.h>
#include <container/deque.h>

namespace hash_map
{
    template<typename DATA, size_t SIZE>
    void initialize(HashMap<DATA, SIZE>& hash_map);
    
    template<typename DATA, size_t SIZE>
    void set(const HashMap<DATA, SIZE>& hash_map, uint64_t key, DATA value);
    
    template<typename DATA, size_t SIZE>
    bool has(const HashMap<DATA, SIZE>& hash_map, uint64_t key);
    
    template<typename DATA, size_t SIZE>
    DATA& get(const HashMap<DATA, SIZE>& hash_map, uint64_t key, DATA& default_obj);
    
    template<typename DATA, size_t SIZE>
    DATA* get_ref(HashMap<DATA, SIZE>& hash_map, uint64_t key);
    
    template<typename DATA, size_t SIZE>
    void remove(const HashMap<DATA, SIZE>& hash_map, uint64_t key);
}

namespace hash_map
{
    const uint32_t END_OF_LIST = 0xffffffffu;
    
    struct FindResult
    {
        uint32_t hash_i;
        uint32_t data_prev;
        uint32_t data_i;
    };
    
    template<typename DATA, size_t SIZE>
    uint32_t add_entry(HashMap<DATA, SIZE>& hash_map, uint64_t key)
    {
        typename HashMap<DATA, SIZE>::HashEntry e;
        e.key = key;
        e.next = END_OF_LIST;
        uint32_t ei = deque::pop_front(hash_map._freelist);
        
        hash_map._data[ei] = e;
        hash_map._num_objects++;
        
        return ei;
    }
    
    template<typename DATA, size_t SIZE>
    FindResult find(const HashMap<DATA, SIZE>& hash_map, uint64_t key)
    {
        FindResult fr;
        fr.hash_i = END_OF_LIST;
        fr.data_prev = END_OF_LIST;
        fr.data_i = END_OF_LIST;

        fr.hash_i = key % SIZE;
        fr.data_i = hash_map._hash[fr.hash_i];
        
        while (fr.data_i != END_OF_LIST)
        {
            if (hash_map._data[fr.data_i].key == key)
                return fr;
            fr.data_prev = fr.data_i;
            fr.data_i = hash_map._data[fr.data_i].next;
        }
        return fr;
    }
    
    template<typename DATA, size_t SIZE>
    void erase(HashMap<DATA, SIZE>& hash_map, const FindResult &fr)
    {
        if (fr.data_prev == END_OF_LIST)
            hash_map._hash[fr.hash_i] = hash_map._data[fr.data_i].next;
        else
            hash_map._data[fr.data_prev].next = hash_map._data[fr.data_i].next;
        
        if (fr.data_i == hash_map._num_objects - 1)
        {
            hash_map._num_objects--;
            return;
        }
        
        uint32_t data_i = fr.data_i;
        deque::push_front(hash_map._freelist, data_i);
        
        hash_map._data[fr.data_i] = hash_map._data[hash_map._num_objects - 1];
        FindResult last = find(hash_map, hash_map._data[fr.data_i].key);
        
        if (last.data_prev != END_OF_LIST)
            hash_map._data[last.data_prev].next = fr.data_i;
        else
            hash_map._hash[last.hash_i] = fr.data_i;
    }
    
    template<typename DATA, size_t SIZE>
    uint32_t find_or_make(HashMap<DATA, SIZE>& hash_map, uint64_t key)
    {
        const FindResult fr = find(hash_map, key);
        if (fr.data_i != END_OF_LIST)
            return fr.data_i;
        
        uint32_t i = add_entry(hash_map, key);
        if (fr.data_prev == END_OF_LIST)
            hash_map._hash[fr.hash_i] = i;
        else
            hash_map._data[fr.data_prev].next = i;
        return i;
    }
    
    template<typename DATA, size_t SIZE>
    void find_and_erase(HashMap<DATA, SIZE>& hash_map, uint64_t key)
    {
        const FindResult fr = find(hash_map, key);
        if (fr.data_i != END_OF_LIST)
            erase(hash_map, fr);
    }
    
    template<typename DATA, size_t SIZE>
    uint32_t find_or_fail(const HashMap<DATA, SIZE>& hash_map, uint64_t key)
    {
        return find(hash_map, key).data_i;
    }
    
    template<typename DATA, size_t SIZE>
    void set(HashMap<DATA, SIZE>& hash_map, uint64_t key, DATA value)
    {
        const uint32_t i = find_or_make(hash_map, key);
        hash_map._data[i].value = value;
    }
    
    
    template<typename DATA, size_t SIZE>
    bool has(const HashMap<DATA, SIZE>& hash_map, uint64_t key)
    {
        return find_or_fail(hash_map, key) != END_OF_LIST;
    }
    
    template<typename DATA, size_t SIZE>
    DATA& get(HashMap<DATA, SIZE>& hash_map, uint64_t key, DATA& default_obj)
    {
        const uint32_t i = find_or_fail(hash_map, key);
        return i == END_OF_LIST ? default_obj : hash_map._data[i].value;
    }
    
    template<typename DATA, size_t SIZE>
    DATA* get_ref(HashMap<DATA, SIZE>& hash_map, uint64_t key)
    {
        const uint32_t i = find_or_fail(hash_map, key);
        return i == END_OF_LIST ? nullptr : &hash_map._data[i].value;
    }
    
    template<typename DATA, size_t SIZE>
    void remove(HashMap<DATA, SIZE>& hash_map, uint64_t key)
    {
        find_and_erase(hash_map, key);
    }
    
    template<typename DATA, size_t SIZE>
    void clear(HashMap<DATA, SIZE>& hash_map)
    {
        for (uint32_t i=0; i< SIZE; ++i)
        {
            hash_map._hash[i] = END_OF_LIST;
            deque::push_back(hash_map._freelist, i);
        }

        hash_map._num_objects = 0;
    }
}
