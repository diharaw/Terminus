#include <io/src/filesystem_impl.hpp>
#include <io/src/os_file.hpp>
#include <io/src/zip_file.hpp>
#include <io/src/zip_directory.hpp>
#include <io/src/disk_directory.hpp>
#include <string>

#include <zip.h>

TE_BEGIN_TERMINUS_NAMESPACE

FileSystem::FileSystem()
{
    for (uint32_t i = 0; i < TE_VIRTUAL_DIR_POOL; i++)
        m_free_slots.push_back(i);
    
    m_root_index = create_virtual_dir(nullptr, "");
    VirtualDirectory* root = &m_virtual_dir_pool[m_root_index];
    IDirectory* root_dir = TE_NEW(&m_allocator) DiskDirectory("");
    root->add_dir(root_dir);
}

FileSystem::~FileSystem()
{
    
}

void FileSystem::vfs_mount_dir(FSNameBuffer dir, FSNameBuffer point)
{
    VirtualDirectory* current = &m_virtual_dir_pool[m_root_index];
    FSNameBuffer remainder = point;
    
    while (true)
    {
        if (remainder == "")
        {
            IDirectory* new_dir = nullptr;
            
            FSNameBuffer ext = file_extension(dir);
            
            if (ext == "zip") // ZIP
                new_dir = TE_NEW(&m_allocator) ZipDirectory(dir);
            else			  // DIRECTORY
                new_dir = TE_NEW(&m_allocator) DiskDirectory(dir);
            
            current->add_dir(new_dir);
            break;
        }
        else
        {
            size_t pos = remainder.find_first('/');
            FSNameBuffer sub_dir = remainder;
            
            if (pos != FSNameBuffer::END)
                sub_dir = remainder.substring(0, pos - 1);
            
            uint64_t sub_dir_hash = TE_HASH(sub_dir.c_str());
            
            uint32_t sub_dir_index = current->get(sub_dir_hash);
            
            if (sub_dir_index == TE_VIRTUAL_DIR_POOL)
                sub_dir_index = create_virtual_dir(current, sub_dir);
            
            current = &m_virtual_dir_pool[sub_dir_index];
            
            if (pos != FSNameBuffer::END)
                remainder = remainder.substring(pos + 1, FSNameBuffer::END);
            else
                remainder = "";
        }
    }
}

IFile* FileSystem::vfs_open_file(FSNameBuffer file, uint32_t mode)
{
    FSNameBuffer path = "";
    FSNameBuffer filename = file;
    
    size_t lpos = file.find_last('/');
    
    if (lpos != FSNameBuffer::END)
    {
        filename = file.substring(lpos + 1, FSNameBuffer::END);
        path = file.substring(0, lpos - 1);
    }
    
    FSNameBuffer append = "";
    VirtualDirectory* vdir = find_virtual_dir(path, append);
    
    if(append != "")
        filename = append + "/" + filename;
    
    if (vdir)
    {
        for (uint32_t i = 0; i < vdir->m_num_dir; i++)
        {
            IDirectory* dir = vdir->m_contents[i];
            
            if (dir)
            {
                IFile* f = dir->open_file(filename, mode, &m_allocator);
                
                if (f)	// File found, early exit.
                    return f;
            }
        }
    }
    
    return nullptr;
}

VirtualDirectory* FileSystem::find_virtual_dir(FSNameBuffer path, FSNameBuffer& append)
{
    VirtualDirectory* current = &m_virtual_dir_pool[m_root_index];
    FSNameBuffer remainder = path;
    
    while (true)
    {
        if (remainder == "")
        {
            append = remainder;
            return current;
        }
        else
        {
            size_t pos = remainder.find_first('/');
            FSNameBuffer sub_dir = remainder;
            
            if (pos != FSNameBuffer::END)
                sub_dir = remainder.substring(0, pos - 1);
            
            uint64_t sub_dir_hash = TE_HASH(sub_dir.c_str());
            
            uint32_t sub_dir_index = current->get(sub_dir_hash);
            
            if (sub_dir_index == TE_VIRTUAL_DIR_POOL)
            {
                append = remainder;
                return current; // can be a sub-folder inside of a zip archive, so return.
            }
            else
            {
                current = &m_virtual_dir_pool[sub_dir_index];
                
                if (pos != FSNameBuffer::END)
                    remainder = remainder.substring(pos + 1, FSNameBuffer::END);
                else
                    remainder = "";
            }
        }
    }
}

FSNameBuffer FileSystem::file_extension(FSNameBuffer file)
{
    size_t pos = file.find_last('.');
    
    if (pos == FSNameBuffer::END)
        return "";
    else
        return file.substring(pos + 1, FSNameBuffer::END);
}

uint32_t FileSystem::create_virtual_dir(VirtualDirectory* parent, FSNameBuffer name)
{
    uint32_t index = m_free_slots.pop_front();
    
    if (parent)
        parent->attach_child(index, name);
    
    VirtualDirectory& child_dir = m_virtual_dir_pool[index];
    child_dir.set_name(name);
    
    return index;
}

TE_END_TERMINUS_NAMESPACE
