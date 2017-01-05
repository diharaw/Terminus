#include "RenderPassCache.h"
#include "../Platform/platform.h"
#include <Core/context.h>

namespace terminus
{
	RenderPassFactory::RenderPassFactory()
	{

	}

	RenderPassFactory::~RenderPassFactory()
	{

	}

	void RenderPassFactory::Initialize()
	{
		
	}

	RenderPass* RenderPassFactory::Create(AssetCommon::TextLoadData* _data)
	{
        RenderDevice& device = context::get_render_device();
        Platform& platform = context::get_platform();
        
		JsonDocument doc;
		doc.Parse(_data->buffer);

		RenderPass* render_pass = new RenderPass();

		if (doc.HasMember("name"))
		{
			render_pass->name = std::string(doc["name"].GetString());
		}

		if (doc.HasMember("render_pass_type"))
		{
			String type = std::string(doc["render_pass_type"].GetString());

			if (type == "SHADOW_MAP")
				render_pass->render_pass_type = RenderPassType::SHADOW_MAP;
			if (type == "GAME_WORLD")
				render_pass->render_pass_type = RenderPassType::GAME_WORLD;
			if (type == "POST_PROCESS")
				render_pass->render_pass_type = RenderPassType::POST_PROCESS;
			if (type == "UI")
				render_pass->render_pass_type = RenderPassType::UI;
			if (type == "DEBUG")
				render_pass->render_pass_type = RenderPassType::DEBUG;
			if (type == "COMPOSITION")
				render_pass->render_pass_type = RenderPassType::COMPOSITION;
		}

		if (doc.HasMember("global_resources"))
		{
			if (doc.HasMember("framebuffers"))
			{
				rapidjson::Value& framebuffers = doc["framebuffers"];

				for (rapidjson::SizeType i = 0; i < framebuffers.Size(); i++)
				{
					rapidjson::Value& value = framebuffers[i]["render_targets"];
					Framebuffer* framebuffer = device.CreateFramebuffer();
					FramebufferInfo fb_info;

					fb_info.name = String(framebuffers[i]["name"].GetString());

					for (rapidjson::SizeType j = 0; j < value.Size(); j++)
					{
						int width = 800;
						int height = 600;
						String target_name = "default_target";
						TextureFormat format = TextureFormat::R8G8B8A8_UNORM;
						RenderTargetInfo target_info;

						if (value[i].HasMember("name"))
						{
							target_name = std::string(value[i]["name"].GetString());
							target_info.name = target_name;
						}
						if (value[i].HasMember("format"))
						{
							String formatStr = std::string(value[i]["format"].GetString());

						    if (formatStr == "R32G32B32_FLOAT")
							    format = TextureFormat::R32G32B32_FLOAT;
							else if (formatStr == "R32G32B32A32_FLOAT")
								format = TextureFormat::R32G32B32A32_FLOAT;
							else if(formatStr == "R32G32B32_UINT")
								format = TextureFormat::R32G32B32_UINT;
							else if (formatStr == "R32G32B32A32_UINT")
								format = TextureFormat::R32G32B32A32_UINT;
							else if (formatStr == "R32G32B32_INT")
								format = TextureFormat::R32G32B32_INT;
							else if (formatStr == "R32G32B32A32_INT")
								format = TextureFormat::R32G32B32A32_INT;
							else if (formatStr == "R16G16B16A16_FLOAT")
								format = TextureFormat::R16G16B16A16_FLOAT;
							else if (formatStr == "R16G16B16A16_UINT")
								format = TextureFormat::R16G16B16A16_UINT;
							else if (formatStr == "R16G16B16A16_INT")
								format = TextureFormat::R16G16B16A16_INT;
							else if (formatStr == "R8G8B8A8_UNORM")
								format = TextureFormat::R8G8B8A8_UNORM;
							else if (formatStr == "R8G8B8A8_UINT")
								format = TextureFormat::R8G8B8A8_UINT;
							else if (formatStr == "D32_FLOAT_S8_UINT")
								format = TextureFormat::D32_FLOAT_S8_UINT;
							else if (formatStr == "D24_FLOAT_S8_UINT")
								format = TextureFormat::D24_FLOAT_S8_UINT;
							else if (formatStr == "D16_FLOAT")
								format = TextureFormat::D16_FLOAT;
							else
								assert(false);
						}
						if (value[i].HasMember("width_divisor"))
						{
							float divisor = value[i]["width_divisor"].GetFloat();
							width = static_cast<float>(platform.get_width()) / divisor;
							target_info.width_divisor = divisor;
						}
						if (value[i].HasMember("height_divisor"))
						{
							float divisor = value[i]["height_divisor"].GetFloat();
							height = static_cast<float>(platform.get_height()) / divisor;
							target_info.height_divisor = divisor;
						}

						Texture2D* render_target = device.CreateTexture2D(width, height, nullptr, format, true);

						if (format == TextureFormat::D32_FLOAT_S8_UINT || format == TextureFormat::D24_FLOAT_S8_UINT || format == TextureFormat::D16_FLOAT)
							device.AttachDepthStencilTarget(framebuffer, render_target);
						else
							device.AttachRenderTarget(framebuffer, render_target);

						device.AddToRenderTargetPool(target_name, render_target);
						fb_info.render_target_info.push_back(target_info);
					}

					render_pass->framebuffers.push_back(framebuffer);
					render_pass->framebuffer_info_list.push_back(fb_info);
					device.AddToFramebufferPool(fb_info.name, framebuffer);
				}
			}
		}

		if (doc.HasMember("sub_passes"))
		{
			rapidjson::Value& sub_passes = doc["sub_passes"];

			for (rapidjson::SizeType i = 0; i < sub_passes.Size(); i++)
			{
				RenderSubPass sub_pass;
				String framebuffer_id = String(sub_passes[i]["framebuffer_target"].GetString());
				sub_pass.framebuffer_target = device.GetFramebufferFromPool(framebuffer_id);

				render_pass->sub_passes.push_back(sub_pass);
			}
		}

		return render_pass;
	}
} // namespace terminus
