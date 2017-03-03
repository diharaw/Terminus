#pragma once

#include <container/hash_map.h>
#include <Utility/murmur_hash.h>
#include <Core/config.h>
#include <types.h>

namespace terminus
{
	enum class ConfigFieldType
	{
		STRING_FIELD = 0,
		BOOL_FIELD,
		INT_FIELD,
		FLOAT_FIELD,
        NO_FIELD
	};

	struct ConfigField
	{
		ConfigFieldType type;
        StringBuffer64 string_value;
		union
		{
			bool		   bool_value;
			int			   int_value;
			float		   float_value;
		};
	};

	class ConfigFile
	{
	private:
		HashMap<ConfigField, MAX_CONFIG_FIELDS> _config_fields;
        bool                                    _valid;

	public:
		ConfigFile();
		~ConfigFile();
        bool            has_value(StringBuffer64 field_name);
        bool            has_value_of_type(StringBuffer64 field_name, ConfigFieldType type);
		StringBuffer64  get_string(StringBuffer64 field_name);
		bool			get_bool(StringBuffer64 field_name);
		int				get_int(StringBuffer64 field_name);
		float			get_float(StringBuffer64 field_name);
        StringBuffer64  get_string(StringBuffer64 field_name, StringBuffer64 default_value);
        bool			get_bool(StringBuffer64 field_name, bool default_value);
        int				get_int(StringBuffer64 field_name, int default_value);
        float			get_float(StringBuffer64 field_name, float default_value);
        void            set_field(StringBuffer64 field_name, ConfigField field);
		void            set_string(StringBuffer64 field_name, StringBuffer64 value);
		void            set_bool(StringBuffer64 field_name, bool value);
		void            set_int(StringBuffer64 field_name, int value);
		void            set_float(StringBuffer64 field_name, float value);
	};
}
