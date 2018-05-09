#pragma once

#include <core/terminus_macros.hpp>
#include <core/terminus_types.hpp>
#include <stl/string_buffer.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

struct DeviceProperties
{
	uint32_t	   vendor_id;
	uint32_t	   driver;
	StringBuffer32 name;
	StringBuffer32 type;
};

struct DeviceLimits
{

};

struct VertexBufferDesc
{

};

TE_END_TERMINUS_NAMESPACE