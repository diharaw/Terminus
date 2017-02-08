#include <Resource/config_file_factory.h>
#include <Core/context.h>

namespace terminus
{
    namespace config_file_factory
    {
        ConfigFile* create(StringBuffer64 file_name)
        {
            ConfigFile* config_file = T_NEW ConfigFile();
            asset_common::TextLoadData* load_data = text_loader::load(String(file_name.c_str()));
            
            if(load_data->buffer)
            {
                JsonDocument doc;
                doc.Parse(load_data->buffer);
                
                for (JsonValue::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr)
                {
                    StringBuffer64 field_name = itr->name.GetString();
                    
                    if(itr->value.IsInt())
                        config_file->set_int(field_name, itr->value.GetInt());
                    else if(itr->value.IsFloat())
                        config_file->set_float(field_name, itr->value.GetFloat());
                    else if(itr->value.IsBool())
                        config_file->set_bool(field_name, itr->value.GetBool());
                    else if(itr->value.IsString())
                        config_file->set_string(field_name, itr->value.GetString());
                    else if(itr->value.IsObject())
                    {
                        
                    }
                    else if(itr->value.IsArray())
                    {
                        
                    }
                }
                
                free(load_data->buffer);
            }
            
            free(load_data);
            
            return config_file;
        }
    }
}
