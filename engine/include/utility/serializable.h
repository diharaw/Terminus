#pragma once

#include <stdint.h>

#include <Core/macro.h>
#include <Utility/murmur_hash.h>

#define SERIALIZABLE(x) mark_serializable(HASH(#x), &x, sizeof(x))
#define MAX_MEMBERS 20

namespace terminus
{
    struct MemberInfo
    {
        size_t   _size;
        uint64_t _hash;
        void*    _ptr;
    };
    
	class Serializable
	{
    private:
        MemberInfo _member_info_array[MAX_MEMBERS];
        uint32_t   _count;
        size_t     _total_size;
        
	public:
		Serializable()
        {
            _total_size = sizeof(uint32_t); // for member count
            _count      = 0;
            
            for (int i = 0; i < MAX_MEMBERS; i++)
            {
                _member_info_array[i]._ptr = nullptr;
                _member_info_array[i]._size = 0;
                _member_info_array[i]._hash = 0;
            }
        }
        
		virtual ~Serializable()
        {
            
        }
        
        inline void mark_serializable(uint64_t hash, void* ptr, size_t size)
        {
            _total_size += size;
            _total_size += sizeof(uint64_t);
            
            _member_info_array[_count]._size = size;
            _member_info_array[_count]._ptr = ptr;
            _member_info_array[_count]._hash = hash;
            
            _count++;
        }
        
		inline void serialize(void* mem)
        {
            char* ptr = (char*)mem;
            
            memcpy(ptr, &_count, sizeof(uint32_t));
            ptr += sizeof(uint32_t);

            for (int i = 0; i < _count; i++)
            {
                memcpy(ptr, &_member_info_array[i]._hash, sizeof(uint64_t));
                ptr += sizeof(uint64_t);
                memcpy(ptr, _member_info_array[i]._ptr, _member_info_array[i]._size);
                ptr += _member_info_array[i]._size;
            }
        }
        
		inline void deserialize(void* data)
        {
            char* ptr = (char*)data;
            
            uint32_t previous_count = 0;
            memcpy(&previous_count, ptr, sizeof(uint32_t));
            ptr += sizeof(uint32_t);
            
            for (int i = 0; i < previous_count; i++)
            {
                uint64_t hash;
                memcpy(&hash, ptr, sizeof(uint64_t));
                ptr += sizeof(uint64_t);
                
                MemberInfo* info = nullptr;
                
                for (int j = 0; j < _count; j++)
                {
                    if(hash == _member_info_array[i]._hash)
                    {
                        info = &_member_info_array[i];
                        break;
                    }
                }
                
                if(info)
                {
                    memcpy(info->_ptr, ptr, info->_size);
                    ptr += info->_size;
                }
                else
                {
                    // data corrupted
                    return;
                }
            }
        }
        
		inline size_t get_size()
        {
            return _total_size;
        }
	};
}
