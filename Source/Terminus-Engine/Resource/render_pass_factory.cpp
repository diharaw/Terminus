#include <Resource/render_pass_factory.h>
#include <Thread/thread_pool.h>
#include <Resource/text_loader.h>
#include <Platform/platform.h>
#include <Graphics/render_target_pool.h>
#include <Graphics/framebuffer_pool.h>
#include <Core/context.h>

namespace terminus
{
    struct AttahDepthStencilTargetTaskData
    {
        Framebuffer* _framebuffer;
        Texture*   _ds_target;
    };
    
    struct AttahRenderTargetTaskData
    {
        Framebuffer* _framebuffer;
        Texture*   _render_target;
    };
    
	namespace render_pass_factory
    {   
        void attach_depth_stencil_target_task(void* data)
        {
            AttahDepthStencilTargetTaskData* task_data = (AttahDepthStencilTargetTaskData*)data;
            RenderDevice& device = context::get_render_device();
            device.AttachDepthStencilTarget(task_data->_framebuffer, task_data->_ds_target);
        }
        
        void attach_render_target_task(void* data)
        {
            AttahRenderTargetTaskData* task_data = (AttahRenderTargetTaskData*)data;
            RenderDevice& device = context::get_render_device();
            device.AttachRenderTarget(task_data->_framebuffer, task_data->_render_target);
        }
        
        RenderPass* create(String render_pass_name)
        {
            Platform& platform = context::get_platform();
            RenderTargetPool& rt_pool = context::get_render_target_pool();
            FramebufferPool& fb_pool = context::get_framebuffer_pool();
            
            asset_common::TextLoadData* load_data = text_loader::load(render_pass_name);
            
            RenderPass* render_pass = new RenderPass();
            
            JsonDocument doc;
            doc.Parse(load_data->buffer);
            
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
                        
                        FramebufferInfo fb_info;
                        fb_info.name = String(framebuffers[i]["name"].GetString());
                        Framebuffer* framebuffer = fb_pool.create(fb_info.name);
                        
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
                            
                            Texture* render_target = rt_pool.create(target_name, width, height, format);
                            
                            if (format == TextureFormat::D32_FLOAT_S8_UINT || format == TextureFormat::D24_FLOAT_S8_UINT || format == TextureFormat::D16_FLOAT)
                            {
                                Task ds_attach_task;
                                AttahDepthStencilTargetTaskData* ds_task_data = task_data<AttahDepthStencilTargetTaskData>(ds_attach_task);
                                ds_task_data->_ds_target = render_target;
                                ds_task_data->_framebuffer = framebuffer;
                                ds_attach_task._function.Bind<&attach_depth_stencil_target_task>();
                                
                                submit_gpu_upload_task(ds_attach_task);
                            }
                            else
                            {
                                Task rt_attach_task;
                                AttahRenderTargetTaskData* rt_task_data = task_data<AttahRenderTargetTaskData>(rt_attach_task);
                                rt_task_data->_render_target = render_target;
                                rt_task_data->_framebuffer = framebuffer;
                                rt_attach_task._function.Bind<&attach_render_target_task>();
                                
                                submit_gpu_upload_task(rt_attach_task);
                            }
                            
                            fb_info.render_target_info.push_back(target_info);
                        }
                        
                        render_pass->framebuffers.push_back(framebuffer);
                        render_pass->framebuffer_info_list.push_back(fb_info);
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
                    sub_pass.framebuffer_target = fb_pool.lookup(framebuffer_id);
                    
                    render_pass->sub_passes.push_back(sub_pass);
                }
            }
            
            return render_pass;
        }
    }
} // namespace terminus
