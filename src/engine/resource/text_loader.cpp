#include <Resource/text_loader.h>
#include <Resource/asset_common.h>
#include <IO/filesystem.h>

namespace terminus
{
    namespace text_loader
    {
        asset_common::TextLoadData* load(String id)
        {
            FileHandle handle = filesystem::read_file(id, true);
            
            if (handle.buffer)
            {
                asset_common::TextLoadData* load_data = new asset_common::TextLoadData();
                
                load_data->buffer = handle.buffer;
                
                return load_data;
            }
            
            return nullptr;
        }
    }
} // namespace terminus
