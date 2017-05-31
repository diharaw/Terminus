#pragma once

#include <stdint.h>
#include <climits>
#include <utility/string_buffer.h>
#include <core/macro.h>
#include <glm.hpp>
#include <string>
#include <array>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <sol.hpp>
#include <atomic>
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"

using ushort		   = unsigned short;
using uchar			   = unsigned char;
using Vector2		   = glm::vec2;
using Vector3		   = glm::vec3;
using Vector4		   = glm::vec4;
using iVector4		   = glm::ivec4;
using Matrix3		   = glm::mat3;
using Matrix4		   = glm::mat4;
using Quaternion	   = glm::quat;
using ResourceHandle   = unsigned;
using String		   = std::string;
using JsonDocument	   = rapidjson::Document;
using JsonValue		   = rapidjson::Value;
using LuaObject		   = sol::table;
using LuaFunction	   = sol::function;
using LuaScriptFile	   = StringBuffer<4096>;
using StringBuffer32   = StringBuffer<32>;
using StringBuffer64   = StringBuffer<64>;
using StringBuffer128  = StringBuffer<128>;
using StringBuffer256  = StringBuffer<256>;
using StringBuffer512  = StringBuffer<512>;
using StringBuffer1024 = StringBuffer<1024>;
using StringBuffer2048 = StringBuffer<2048>;
using StringBuffer4096 = StringBuffer<4096>;

struct HashResult
{
    uint64_t hash;
};

class TypeID
{
    static std::atomic_int counter;

public:
	template<typename T>
	static size_t value()
	{
		static size_t id = counter++;
		return id;
	}
};