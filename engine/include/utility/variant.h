#pragma once

#include <core/types.h>

TERMINUS_BEGIN_NAMESPACE

enum VariantType
{
	VARIANT_NONE = 0,
	VARIANT_INT8,       // 1-byte
	VARIANT_INT16,      // 2-byte
	VARIANT_INT32,      // 4-byte
	VARIANT_INT64,      // 8-byte
	VARIANT_UINT8,      // 1-byte
	VARIANT_UINT16,     // 2-byte
	VARIANT_UINT32,     // 4-byte
	VARIANT_UINT64,     // 8-byte
	VARIANT_FLOAT,      // 4-byte
	VARIANT_DOUBLE,     // 8-bytet
	VARIANT_VECTOR2,    // 8-byte
	VARIANT_VECTOR3,    // 12-byte
	VARIANT_VECTOR4,    // 16-byte
};

struct Variant
{
	union
	{
		int8_t   data_int8;
		int16_t  data_int16;
		int32_t  data_int32;
		int64_t  data_int64;
		uint8_t  data_uint8;
		uint16_t data_uint16;
		uint32_t data_uint32;
		uint64_t data_uint64;
		float	 data_float;
		double	 data_double;
		Vector2  data_vec2;
		Vector3  data_vec3;
		Vector4  data_vec4;
		void*	 data_ptr;
	};
	VariantType type;
	size_t size;

	Variant(int8_t _value)
	{
		data_int8 = _value;
		type = VARIANT_INT8;
		size = sizeof(_value);
	}

	Variant(int16_t _value)
	{
		data_int32 = _value;
		type = VARIANT_INT16;
		size = sizeof(_value);
	}

	Variant(int32_t _value)
	{
		data_int16 = _value;
		type = VARIANT_INT16;
		size = sizeof(_value);
	}

	Variant(int64_t _value)
	{
		data_int64 = _value;
		type = VARIANT_INT64;
		size = sizeof(_value);
	}

	Variant(uint8_t _value)
	{
		data_uint8 = _value;
		type = VARIANT_UINT8;
		size = sizeof(_value);
	}

	Variant(uint16_t _value)
	{
		data_uint32 = _value;
		type = VARIANT_UINT16;
		size = sizeof(_value);
	}

	Variant(uint32_t _value)
	{
		data_uint16 = _value;
		type = VARIANT_UINT16;
		size = sizeof(_value);
	}

	Variant(uint64_t _value)
	{
		data_uint64 = _value;
		type = VARIANT_UINT64;
		size = sizeof(_value);
	}

	Variant(float _value)
	{
		data_float = _value;
		type = VARIANT_FLOAT;
		size = sizeof(_value);
	}

	Variant(double _value)
	{
		data_double = _value;
		type = VARIANT_DOUBLE;
		size = sizeof(_value);
	}

	Variant(Vector2 _value)
	{
		data_vec2 = _value;
		type = VARIANT_VECTOR2;
		size = sizeof(_value);
	}

	Variant(Vector3 _value)
	{
		data_vec2 = _value;
		type = VARIANT_VECTOR3;
		size = sizeof(_value);
	}

	Variant(Vector3 _value)
	{
		data_vec2 = _value;
		type = VARIANT_VECTOR3;
		size = sizeof(_value);
	}
};

TERMINUS_END_NAMESPACE