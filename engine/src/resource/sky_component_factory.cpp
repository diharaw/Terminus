#include <resource/sky_component_factory.h>
#include <gameplay/component_types.h>
#include <core/context.h>

namespace terminus
{
    namespace sky_component_factory
    {
        struct CreateSamplerTaskData
        {
            SamplerState** sampler;
        };
        
        void create_sampler_task(void* data)
        {
            CreateSamplerTaskData* task_data = (CreateSamplerTaskData*)data;
            
            SamplerStateCreateDesc desc;
            
            desc.min_filter = TextureFilteringMode::LINEAR_ALL;
            desc.mag_filter = TextureFilteringMode::LINEAR_ALL;
            desc.wrap_mode_u = TextureWrapMode::CLAMP_TO_EDGE;
            desc.wrap_mode_v = TextureWrapMode::CLAMP_TO_EDGE;
            desc.wrap_mode_w = TextureWrapMode::CLAMP_TO_EDGE;
            
            *task_data->sampler = context::get_render_device().create_sampler_state(desc);
        }

        
        void create(JsonValue& json, Entity& entity, Scene* scene)
        {
            SkyComponent& component = scene->attach_sky_component(entity);
            TextureCache& cache = context::get_texture_cache();
            
            component.cubemap = json["cubemap"].GetString();
            component.texture = cache.load_cubemap(std::string(component.cubemap.c_str()));
            
			Renderer* renderer = &context::get_renderer();
			Task* task = renderer->create_upload_task();
            CreateSamplerTaskData* sampler_task_data = task_data<CreateSamplerTaskData>(task);
            
            sampler_task_data->sampler = &component.sampler;
            task->_function.Bind<&create_sampler_task>();
            renderer->enqueue_upload_task(task);
        }
    }
}
