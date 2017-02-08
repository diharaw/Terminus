#include <Core/config_file.h>

namespace terminus
{
    ConfigFile::ConfigFile()
    {
        _valid = false;
    }
    
    ConfigFile::~ConfigFile()
    {
        
    }
    
    void ConfigFile::set_field(StringBuffer64 field_name, ConfigField field)
    {
        _valid = true;
        _config_fields.set(HASH(field_name.c_str()), field);
    }
    
    bool ConfigFile::has_value(StringBuffer64 field_name)
    {
        return _config_fields.has(HASH(field_name.c_str()));
    }
    
    bool ConfigFile::has_value_of_type(StringBuffer64 field_name, ConfigFieldType type)
    {
        ConfigField field;
        
        if(_config_fields.get(HASH(field_name.c_str()), field))
        {
            if(field.type == type)
                return true;
            else
                return false;
        }
        else
            return false;
    }
    
    StringBuffer64 ConfigFile::get_string(StringBuffer64 field_name)
    {
        ConfigField field;
        _config_fields.get(HASH(field_name.c_str()), field);
        
        return field.string_value;
    }
    
    bool ConfigFile::get_bool(StringBuffer64 field_name)
    {
        ConfigField field;
        _config_fields.get(HASH(field_name.c_str()), field);
        
        return field.bool_value;
    }
    
    int	ConfigFile::get_int(StringBuffer64 field_name)
    {
        ConfigField field;
        _config_fields.get(HASH(field_name.c_str()), field);
        
        return field.int_value;
    }
    
    float ConfigFile::get_float(StringBuffer64 field_name)
    {
        ConfigField field;
        _config_fields.get(HASH(field_name.c_str()), field);
        
        return field.float_value;
    }
    
    StringBuffer64 ConfigFile::get_string(StringBuffer64 field_name, StringBuffer64 default_value)
    {
        ConfigField field;
        
        if(_config_fields.get(HASH(field_name.c_str()), field))
        {
            if(field.type == ConfigFieldType::STRING_FIELD)
                return field.string_value;
        }
        
        return default_value;
    }
    
    bool ConfigFile::get_bool(StringBuffer64 field_name, bool default_value)
    {
        ConfigField field;
        
        if(_config_fields.get(HASH(field_name.c_str()), field))
        {
            if(field.type == ConfigFieldType::BOOL_FIELD)
                return field.bool_value;
        }
        
        return default_value;
    }
    
    int	ConfigFile::get_int(StringBuffer64 field_name, int default_value)
    {
        ConfigField field;
        
        if(_config_fields.get(HASH(field_name.c_str()), field))
        {
            if(field.type == ConfigFieldType::INT_FIELD)
                return field.int_value;
        }
        
        return default_value;
    }
    
    float ConfigFile::get_float(StringBuffer64 field_name, float default_value)
    {
        ConfigField field;
        
        if(_config_fields.get(HASH(field_name.c_str()), field))
        {
            if(field.type == ConfigFieldType::FLOAT_FIELD)
                return field.float_value;
        }
        
        return default_value;
    }
    
    void ConfigFile::set_string(StringBuffer64 field_name, StringBuffer64 value)
    {
        _valid = true;
        ConfigField field;
        field.string_value = value;
        field.type = ConfigFieldType::STRING_FIELD;
        
        _config_fields.set(HASH(field_name.c_str()), field);
    }
    
    void ConfigFile::set_bool(StringBuffer64 field_name, bool value)
    {
        _valid = true;
        ConfigField field;
        field.bool_value = value;
        field.type = ConfigFieldType::BOOL_FIELD;
        
        _config_fields.set(HASH(field_name.c_str()), field);
    }
    
    void ConfigFile::set_int(StringBuffer64 field_name, int value)
    {
        _valid = true;
        ConfigField field;
        field.int_value = value;
        field.type = ConfigFieldType::INT_FIELD;
        
        _config_fields.set(HASH(field_name.c_str()), field);
    }
    
    void ConfigFile::set_float(StringBuffer64 field_name, float value)
    {
        _valid = true;
        ConfigField field;
        field.float_value = value;
        field.type = ConfigFieldType::FLOAT_FIELD;
        
        _config_fields.set(HASH(field_name.c_str()), field);
    }
    
}
