#include <resource/text_loader.h>
#include <resource/asset_common.h>
#include <io/filesystem.h>

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
