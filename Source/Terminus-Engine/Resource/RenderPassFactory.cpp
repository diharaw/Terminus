#include "RenderPassCache.h"
#include "../Platform/PlatformBackend.h"

namespace Terminus { namespace Resource {

	RenderPassFactory::RenderPassFactory()
	{

	}

	RenderPassFactory::~RenderPassFactory()
	{

	}

	void RenderPassFactory::Initialize(Graphics::RenderDevice* device)
	{
		m_device = device;
	}

	Graphics::RenderPass* RenderPassFactory::Create(AssetCommon::TextLoadData* _data)
	{
		JsonDocument doc;
		doc.Parse(_data->buffer);

		Graphics::RenderPass* render_pass = new Graphics::RenderPass();

		if (doc.HasMember("name"))
		{
			render_pass->name = std::string(doc["name"].GetString());
		}

		if (doc.HasMember("render_pass_type"))
		{
			String type = std::string(doc["render_pass_type"].GetString());

			if (type == "SHADOW_MAP")
				render_pass->render_pass_type = Graphics::RenderPassType::SHADOW_MAP;
			if (type == "GAME_WORLD")
				render_pass->render_pass_type = Graphics::RenderPassType::GAME_WORLD;
			if (type == "POST_PROCESS")
				render_pass->render_pass_type = Graphics::RenderPassType::POST_PROCESS;
			if (type == "UI")
				render_pass->render_pass_type = Graphics::RenderPassType::UI;
			if (type == "DEBUG")
				render_pass->render_pass_type = Graphics::RenderPassType::DEBUG;
			if (type == "COMPOSITION")
				render_pass->render_pass_type = Graphics::RenderPassType::COMPOSITION;
		}

		if (doc.HasMember("global_resources"))
		{
			if (doc.HasMember("framebuffers"))
			{
				rapidjson::Value& framebuffers = doc["framebuffers"];

				for (rapidjson::SizeType i = 0; i < framebuffers.Size(); i++)
				{
					rapidjson::Value& value = framebuffers[i]["render_targets"];
					Graphics::Framebuffer* framebuffer = m_device->CreateFramebuffer();
					Graphics::FramebufferInfo fb_info;

					fb_info.name = String(framebuffers[i]["name"].GetString());

					for (rapidjson::SizeType j = 0; j < value.Size(); j++)
					{
						int width = 800;
						int height = 600;
						String target_name = "default_target";
						TextureFormat format = TextureFormat::R8G8B8A8_UNORM;
						Graphics::RenderTargetInfo target_info;

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
							width = ((float)PlatformBackend::GetWidth()) / divisor;
							target_info.width_divisor = divisor;
						}
						if (value[i].HasMember("height_divisor"))
						{
							float divisor = value[i]["height_divisor"].GetFloat();
							height = ((float)PlatformBackend::GetHeight()) / divisor;
							target_info.height_divisor = divisor;
						}

						Graphics::Texture2D* render_target = m_device->CreateTexture2D(width, height, nullptr, format, true);

						if (format == TextureFormat::D32_FLOAT_S8_UINT || format == TextureFormat::D24_FLOAT_S8_UINT || format == TextureFormat::D16_FLOAT)
							m_device->AttachDepthStencilTarget(framebuffer, render_target);
						else
							m_device->AttachRenderTarget(framebuffer, render_target);

						m_device->AddToRenderTargetPool(target_name, render_target);
						fb_info.render_target_info.push_back(target_info);
					}

					render_pass->framebuffers.push_back(framebuffer);
					render_pass->framebuffer_info_list.push_back(fb_info);
					m_device->AddToFramebufferPool(fb_info.name, framebuffer);
				}
			}
		}

		if (doc.HasMember("sub_passes"))
		{
			rapidjson::Value& sub_passes = doc["sub_passes"];

			for (rapidjson::SizeType i = 0; i < sub_passes.Size(); i++)
			{
				Graphics::RenderSubPass sub_pass;
				String framebuffer_id = String(sub_passes[i]["framebuffer_target"].GetString());
				sub_pass.framebuffer_target = m_device->GetFramebufferFromPool(framebuffer_id);

				render_pass->sub_passes.push_back(sub_pass);
			}
		}

		return render_pass;
	}

} }
