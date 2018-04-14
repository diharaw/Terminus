#pragma once

#define TE_MAX_DIRECTORIES   20
#define TE_VIRTUAL_DIR_POOL  32
#define TE_MAX_FILENAME_SIZE 128

#define TE_MAX_PLAYERS 8
#define TE_MAX_GAMEPADS 8
#define TE_MAX_JOYSTICKS 8
#define TE_UNASSIGNED_INPUT -1

#define TE_DECLARE_SERIALIZE template<typename T> void serialize(T& serializer)
#define TE_DECLARE_DESERIALIZE template<typename T> void deserialize(T& serializer)

#define TE_SERIALIZE_SIMPLE(value) serializer.serialize_simple(#value, value)
#define TE_SERIALIZE_STRING(value) serializer.serialize_string(#value, value)
#define TE_SERIALIZE_CSTRING(value) serializer.serialize_string(#value, value)
#define TE_SERIALIZE_COMPLEX(value) serializer.serialize_complex(#value, value)
#define TE_SERIALIZE_SIMPLE_ARRAY(value, size) serializer.serialize_simple_array(#value, &value[0], size)
#define TE_SERIALIZE_COMPLEX_ARRAY(value, size) serializer.serialize_complex_array(#value, &value[0], size)

#define TE_DESERIALIZE_SIMPLE(value) serializer.deserialize_simple(#value, value)
#define TE_DESERIALIZE_STRING(value) serializer.deserialize_string(#value, value)
#define TE_DESERIALIZE_CSTRING(value) serializer.deserialize_string(#value, &value)
#define TE_DESERIALIZE_COMPLEX(value) serializer.deserialize_complex(#value, value)
#define TE_DESERIALIZE_SIMPLE_ARRAY(value, size) serializer.deserialize_simple_array(#value, &value[0], size)
#define TE_DESERIALIZE_COMPLEX_ARRAY(value, size) serializer.deserialize_complex_array(#value, &value[0], size)
