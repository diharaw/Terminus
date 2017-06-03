#pragma once

#include <string>
#include <vector>
#include <core/types.h>
#include <graphics/render_common.h>

namespace terminus
{
	#define MAX_SUB_PASSES 16

	struct RenderSubPass;
	struct Texture2D;
	struct Framebuffer;
    struct ShaderProgram;
    struct PipelineStateObject;
	struct FramebufferInfo;
	struct RenderTargetInfo;

	enum SystemUniforms
	{
		SU_VIEW_POSITION = 0,
		SU_VIEW_RAY,
		SU_LIGHT_INFO, // Spot, Directional, Point Light information
	};

	enum class SubPassType
	{
		SHADOW_MAP = 0,
		SCENE,
		SKY,
		POST_PROCESS,
		UI,
		DEBUG,
		COMPOSITION
	};

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
        String               vs_template;
        String               ps_template;
		Framebuffer*         framebuffer_target;
        ShaderProgram*       program;
        PipelineStateObject* pso;
		SubPassType			 sub_pass_type;
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
        uint16_t            num_sub_passes;
		RenderSubPass       sub_passes[MAX_SUB_PASSES];
		FramebufferInfoList framebuffer_info_list;
        
        RenderPass()
        {
            num_sub_passes = 0;
        }
	};
}
