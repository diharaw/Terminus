#pragma once

#include <container/hash_map.h>
#include <Utility/murmur_hash.h>
#include <types.h>

#define MAX_CONFIG_FIELDS		  20

#define CONFIG_APP_NAME			  "app_name"
#define CONFIG_APP_VERSION		  "app_version"
#define CONFIG_ENGINE_VERSION	  "engine_version"
#define CONFIG_ENGINE_BUILD		  "engine_build"
#define CONFIG_INITIAL_SCENE	  "initial_scene"
#define CONFIG_WINDOW_TITLE		  "window_title"
#define CONFIG_WINDOW_WIDTH		  "window_width"
#define CONFIG_WINDOW_HEIGHT	  "window_height"
#define CONFIG_WINDOW_MODE		  "window_mode"
#define CONFIG_WINDOW_VSYNC		  "window_vsync"
#define CONFIG_LOCAL_DATA_DIR	  "local_data_dir"
#define CONFIG_MESH_DIR			  "mesh_dir"
#define CONFIG_MATERIAL_DIR		  "material_dir"
#define CONFIG_TEXTURE_DIR		  "texture_dir"
#define CONFIG_SCRIPT_DIR		  "script_dir"
#define CONFIG_INPUT_MAP_DIR	  "input_map_dir"
#define CONFIG_RENDERING_PATH_DIR "rendering_path_dir"
#define CONFIG_ANIMATION_DIR	  "animation_dir"
#define CONFIG_SCENE_DIR		  "scene_dir"

namespace terminus
{
	enum class ConfigFieldType
	{
		STRING = 0,
		BOOL,
		INT,
		FLOAT
	};

	struct ConfigField
	{
		ConfigFieldType type;
		union
		{
			StringBuffer64 string_value;
			bool		   bool_value;
			int			   int_value;
			float		   float_value;
		};
	};

	class EngineConfig
	{
	private:
		HashMap<ConfigField, MAX_CONFIG_FIELDS> _config_fields;

	public:
		EngineConfig();
		~EngineConfig();
		StringBuffer64& get_string(StringBuffer64 field_name);
		bool			get_bool(StringBuffer64 field_name);
		int				get_int(StringBuffer64 field_name);
		float			get_float(StringBuffer64 field_name);
		void            set_string(StringBuffer64 field_name, StringBuffer64 value);
		void            set_bool(StringBuffer64 field_name, bool value);
		void            set_int(StringBuffer64 field_name, int value);
		void            set_float(StringBuffer64 field_name, float value);
	};
}