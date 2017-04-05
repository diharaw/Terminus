#include <resource/material_factory.h>
#include <resource/text_loader.h>
#include <resource/texture_cache.h>
#include <core/context.h>

namespace terminus
{
	namespace material_factory
    {
        struct CreateSamplerTaskData
        {
            SamplerState** sampler;
        };
        
        void create_sampler_task(void* data)
        {
            CreateSamplerTaskData* task_data = (CreateSamplerTaskData*)data;
            
            SamplerStateCreateDesc desc;
            
            desc.min_filter = TextureFilteringMode::NEAREST_ALL;
            desc.mag_filter = TextureFilteringMode::LINEAR_ALL;
            desc.wrap_mode_u = TextureWrapMode::REPEAT;
            desc.wrap_mode_v = TextureWrapMode::REPEAT;
            desc.wrap_mode_w = TextureWrapMode::REPEAT;
            
            *task_data->sampler = context::get_render_device().create_sampler_state(desc);
        }

        
        Material* create(String material_name)
        {
            TextureCache& cache = context::get_texture_cache();
            String file_name = filesystem::get_file_name_and_extention(material_name);
            
            asset_common::TextLoadData* data = text_loader::load(file_name);
            
            if(!data)
                return nullptr;
            
            JsonDocument doc;
            doc.Parse(data->buffer);
            
            Material* material = new Material();
            
            if (doc.HasMember("diffuse_map"))
            {
                String key = std::string(doc["diffuse_map"].GetString());
                material->texture_maps[DIFFUSE] = (Texture2D*)cache.load(key);
            }
            else
            {
                assert(doc.HasMember("diffuse_value"));
                rapidjson::Value& val = doc["diffuse_value"];
                
                Vector4 color;
                color.r = val["r"].GetFloat();
                color.g = val["g"].GetFloat();
                color.b = val["b"].GetFloat();
                color.a = val["a"].GetFloat();
                
                material->diffuse_value = color;
            }
            
            if (doc.HasMember("normal_map"))
            {
                String key = std::string(doc["normal_map"].GetString());
                material->texture_maps[NORMAL] = (Texture2D*)cache.load(key);
            }
            
            if (doc.HasMember("roughness_map"))
            {
                String key = std::string(doc["roughness_map"].GetString());
                //material->texture_maps[ROUGHNESS] = (Texture2D*)cache.load(key);
            }
            else
            {
                assert(doc.HasMember("roughness_value"));
                material->roughness_value = doc["roughness_value"].GetFloat();
            }
            
            if (doc.HasMember("metalness_map"))
            {
                String key = std::string(doc["metalness_map"].GetString());
                //material->texture_maps[METALNESS] = (Texture2D*)cache.load(key);
            }
            else
            {
                assert(doc.HasMember("metalness_value"));
                material->metalness_value = doc["metalness_value"].GetFloat();
            }
            
            if (doc.HasMember("displacement_map"))
            {
                String key = std::string(doc["displacement_map"].GetString());
                material->texture_maps[DISPLACEMENT] = (Texture2D*)cache.load(key);
            }
            
            Task task;
            CreateSamplerTaskData* sampler_task_data = task_data<CreateSamplerTaskData>(task);
        
            sampler_task_data->sampler = &material->sampler;
            task._function.Bind<&create_sampler_task>();
            submit_gpu_upload_task(task);
            
            return material;
        }
    }
}
