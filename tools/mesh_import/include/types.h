#pragma once

#include <stdint.h>
#include <glm.hpp>
#include <string>
#include <array>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>
#include <json.hpp>

#define T_SAFE_DELETE(x) if(x){ delete x; x = nullptr; }
#define T_SAFE_DELETE_ARRAY(x) if(x) { delete[] x; x = nullptr; }

using uint = unsigned int;
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;
using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
using ushort = unsigned short;
using uchar = unsigned char;
using Vector2 = glm::vec2;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;
using iVector4 = glm::ivec4;
using Matrix3 = glm::mat3;
using Matrix4 = glm::mat4;
using Quaternion = glm::quat;
using ResourceHandle = unsigned;
using String = std::string;
using Json = nlohmann::json;
