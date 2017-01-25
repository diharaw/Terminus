#pragma once

#include <cassert>
#include <container/container_types.h>

namespace deque
{
    template<typename DATA, size_t SIZE>
    void clear(Deque<DATA, SIZE>& deque)
    {
        deque._num_objects = 0;
        deque._front = -1;
        deque._back = 0;
    }
    
    template<typename DATA, size_t SIZE>
    DATA& front(Deque<DATA, SIZE>& deque) { return deque._data[deque._front]; }
    
    template<typename DATA, size_t SIZE>
    DATA& back(Deque<DATA, SIZE>& deque) { return deque._data[deque._back]; }
    
    template<typename DATA, size_t SIZE>
    void push_back(Deque<DATA, SIZE>& deque, DATA& value)
    {
        if(deque._num_objects < SIZE)
        {
            if(deque._back == SIZE && deque._front > 0)
                deque._back = 0;
            
            deque._data[deque._back++] = value;
            deque._num_objects++;
        }
    }
    
    template<typename DATA, size_t SIZE>
    DATA pop_back(Deque<DATA, SIZE>& deque)
    {
        assert(deque._num_objects > 0 && "No elements in Deque to pop");
        
        if(deque._back == 0 && deque._front < SIZE - 1)
            deque._back = SIZE - 1;
        
        deque._num_objects--;
        return deque._data[--deque._back];
    }
    
    template<typename DATA, size_t SIZE>
    void push_front(Deque<DATA, SIZE>& deque, DATA& value)
    {
        if(deque._num_objects < SIZE)
        {
            if(deque._front == -1 && deque._back < SIZE - 1)
                deque._front = SIZE - 1;
            
            deque._data[deque._front--] = value;
            deque._num_objects++;
        }
    }
    
    template<typename DATA, size_t SIZE>
    DATA pop_front(Deque<DATA, SIZE>& deque)
    {
        assert(deque._num_objects > 0 && "No elements in Deque to pop");

        if(deque._front == SIZE && deque._back > 0)
            deque._front = 0;
        
        deque._num_objects--;
        return deque._data[++deque._front];
    }

}
