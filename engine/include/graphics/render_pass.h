#pragma once

#ifndef  RENDERPASS_H
#define RENDERPASS_H

#include <string>
#include <vector>
#include <core/types.h>
#include <graphics/render_common.h>

namespace terminus
{
	using TextureHandle = uint16_t;

	struct RenderSubPass;
	struct Texture2D;
	struct Framebuffer;
    struct ShaderProgram;
	struct FramebufferInfo;
	struct RenderTargetInfo;

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

	enum class RenderPassType
	{
		SHADOW_MAP = 0,
		GAME_WORLD,
		POST_PROCESS,
		UI,
		DEBUG,
		COMPOSITION
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
	using RenderSubPassList = std::vector<RenderSubPass>;
	using RenderTargetInfoList = std::vector<RenderTargetInfo>;
	using FramebufferList = std::vector<Framebuffer*>;
	using FramebufferInfoList = std::vector<FramebufferInfo>;

	//struct RenderSubPass
	//{
	//	ResourceHandle m_framebuffer;
	//	HandleList m_RenderTargetList;
	//	std::vector<char> m_NameChar;
	//	std::string m_Name;

	//	// VS Resources

	//	VariantList     m_UniformListVS;
	//	char*           m_UniformDataVS;
	//	size_t          m_UniformDataSizeVS;
	//	HandleList		m_UnformSamplerLisVS;

	//	// GS Resources

	//	VariantList     m_UniformListGS;
	//	char*           m_UniformDataGS;
	//	size_t          m_UniformDataSizeGS;
	//	HandleList		m_UnformSamplerLisGS;

	//	// TCS Resources

	//	VariantList     m_UniformListTCS;
	//	char*           m_UniformDataTCS;
	//	size_t          m_UniformDataSizeTCS;
	//	HandleList		m_UnformSamplerLisTCS;

	//	// TES Resources

	//	VariantList     m_UniformListTES;
	//	char*           m_UniformDataTES;
	//	size_t          m_UniformDataSizeTES;
	//	HandleList		m_UnformSamplerLisTES;

	//	// PS Resources

	//	VariantList     m_UniformListPS;
	//	char*              m_UniformDataPS;
	//	size_t              m_UniformDataSizePS;
	//	HandleList		m_UnformSamplerLisPS;
	//};

	struct RenderSubPass
	{
        String          vs_template;
        String          ps_template;
		Framebuffer*    framebuffer_target;
        ShaderProgram*  program;
	};

	struct RenderTargetInfo
	{
		float width_divisor;
		float height_divisor;
		std::string name;
	};

	struct FramebufferInfo
	{
		String name;
		std::vector<RenderTargetInfo> render_target_info;
	};

	struct RenderPass
	{
        uint16              pass_id;
		String				name;
		RenderPassType	    render_pass_type;
        GeometryType        geometry_type;
		RenderSubPassList   sub_passes;
		FramebufferList		framebuffers;
		FramebufferInfoList framebuffer_info_list;
	};
}

#endif
