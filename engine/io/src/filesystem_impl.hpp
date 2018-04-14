#pragma once

#include <io/include/filesystem.hpp>
#include <stl/include/murmur_hash.hpp>
#include <stl/include/hash_map.hpp>
#include <memory/src/buffer_linear_allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

using DirectoryMap = HashMap<IDirectory*, TE_MAX_DIRECTORIES>;
using FreeSlotList = Deque<uint32_t, TE_VIRTUAL_DIR_POOL>;

struct VirtualDirectory
{
    using IndexMap = HashMap<uint32_t, TE_MAX_DIRECTORIES>;
    
    uint32_t	 m_num_dir;
    IndexMap	 m_virtual_dir_index_map;
    IDirectory*  m_contents[TE_MAX_DIRECTORIES];
    FSNameBuffer m_name;
    
    VirtualDirectory() : m_num_dir(0), m_name("") // root dir
    {
        for (int i = 0; i < TE_VIRTUAL_DIR_POOL; i++)
            m_virtual_dir_index_map._data[i].value = TE_VIRTUAL_DIR_POOL;
    }
    
    VirtualDirectory(FSNameBuffer name) : m_num_dir(0), m_name(name)
    {
        
    }
    
    inline void set_name(FSNameBuffer name) { m_name = name; }
    
    inline FSNameBuffer name() { return m_name; }
    
    inline void add_dir(IDirectory* dir)
    {
        m_contents[m_num_dir] = dir;
        m_num_dir++;
    }
    
    inline void attach_child(uint32_t index, FSNameBuffer name)
    {
        uint64_t sub_dir_hash = TE_HASH(name.c_str());
        m_virtual_dir_index_map.set(sub_dir_hash, index);
    }
    
    inline uint32_t get(uint64_t hash)
    {
        uint32_t obj = TE_VIRTUAL_DIR_POOL;
        m_virtual_dir_index_map.get(hash, obj);
        return obj;
    }
};

class FileSystem : public IFileSystem
{
public:
    FileSystem();
    ~FileSystem();
    void         vfs_mount_dir(FSNameBuffer dir, FSNameBuffer point) override;
    IFile*       vfs_open_file(FSNameBuffer file, uint32_t mode) override;
    FSNameBuffer file_extension(FSNameBuffer file) override;
    
private:
    VirtualDirectory* find_virtual_dir(FSNameBuffer path, FSNameBuffer& append);
    uint32_t create_virtual_dir(VirtualDirectory* parent, FSNameBuffer name);
    
private:
    DirectoryMap                          m_directory_map;
    VirtualDirectory                      m_virtual_dir_pool[TE_VIRTUAL_DIR_POOL];
    FreeSlotList                          m_free_slots;
    uint32_t                              m_root_index;
    BufferLinearAllocator<TE_MEGABYTE(2)> m_allocator;

};

TE_END_TERMINUS_NAMESPACE
