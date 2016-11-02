#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <vector>
#include "../Types.h"
#include "RenderCommon.h"

using TextureHandle = uint16_t;

struct RenderSubPass;

enum SystemUniforms
{
	SU_VIEW_POSITION = 0,
	SU_VIEW_RAY,
	SU_LIGHT_INFO, // Spot, Directional, Point Light information
};

enum VariantType
{
	VARIANT_NONE = 0,
	VARIANT_INT,        // 4-byte
	VARIANT_UINT8,      // 1-byte
	VARIANT_UINT16,     // 2-byte
	VARIANT_UINT32,     // 4-byte
	VARIANT_UINT64,     // 8-byte
	VARIANT_FLOAT,      // 4-byte
	VARIANT_DOUBLE,     // 8-byte
	VARIANT_UINT,       // 4-byte
	VARIANT_VECTOR2,    // 8-byte
	VARIANT_VECTOR3,    // 12-byte
	VARIANT_VECTOR4,    // 16-byte
};

struct Variant
{
	void* data;
	VariantType type;
	size_t size;

	Variant(int* _value)
	{
		data = (void*)_value;
		type = VARIANT_INT;
		size = sizeof(*_value);
	}

	Variant(float* _value)
	{
		data = (void*)_value;
		type = VARIANT_FLOAT;
		size = sizeof(*_value);
	}

	Variant(double* _value)
	{
		data = (void*)_value;
		type = VARIANT_DOUBLE;
		size = sizeof(*_value);
	}
};

using VariantList = std::vector<Variant>;
using HandleList = std::vector<ResourceHandle>;
using RenderSubPassList = std::vector<RenderSubPass*>;

struct RenderSubPass
{
	ResourceHandle m_framebuffer;
	HandleList m_RenderTargetList;
    std::vector<char> m_NameChar;
    std::string m_Name;

	// VS Resources

	VariantList     m_UniformListVS;
	char*           m_UniformDataVS;
	size_t          m_UniformDataSizeVS;
	HandleList		m_UnformSamplerLisVS;

	// GS Resources

	VariantList     m_UniformListGS;
	char*           m_UniformDataGS;
	size_t          m_UniformDataSizeGS;
	HandleList		m_UnformSamplerLisGS;

	// TCS Resources

	VariantList     m_UniformListTCS;
	char*           m_UniformDataTCS;
	size_t          m_UniformDataSizeTCS;
	HandleList		m_UnformSamplerLisTCS;

	// TES Resources

	VariantList     m_UniformListTES;
	char*           m_UniformDataTES;
	size_t          m_UniformDataSizeTES;
	HandleList		m_UnformSamplerLisTES;

	// PS Resources

	VariantList     m_UniformListPS;
	char*           m_UniformDataPS;
	size_t          m_UniformDataSizePS;
	HandleList		m_UnformSamplerLisPS;
};

struct RenderTargetInfo
{
    std::vector<char> m_NameChar;
    std::string m_Name;
};

struct FramebufferInfo
{
    std::vector<char> m_NameChar;
    std::string m_Name;
    int m_Width;
    int m_Height;
    std::vector<RenderTargetInfo*> m_RenderTargets;
};

struct RenderPass
{
    std::vector<char> m_NameChar;
	std::string m_Name;
	RenderSubPassList m_SubPasses;
	GeometryType m_geometry;
    std::vector<FramebufferInfo*> m_Framebuffers;
};

#endif
