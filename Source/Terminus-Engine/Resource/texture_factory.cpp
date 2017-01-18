#include <Resource/texture_factory.h>
#include <Resource/stb_image_loader.h>
#include <Core/context.h>

namespace terminus
{
    namespace texture_factory
    {
        struct CreateTextureTaskData
        {
            asset_common::ImageLoadData* load_data;
            Texture2D**                   texture;
        };
        
        void create_texture_task(void* data)
        {
            CreateTextureTaskData* task_data = (CreateTextureTaskData*)data;
            *task_data->texture = context::get_render_device().CreateTexture2D(task_data->load_data->width,
                                                                               task_data->load_data->height,
                                                                               task_data->load_data->bytes,
                                                                               TextureFormat::R8G8B8A8_UNORM,
                                                                               false);
            
            free(task_data->load_data->bytes);
            free(task_data->load_data);
        }
        
        Texture* create(String image_name)
        {
            asset_common::ImageLoadData* data = stb_image_loader::load(image_name);
            
            if(!data)
                return nullptr;
            
            Task task;
            Texture2D* texture;
            
            CreateTextureTaskData* texture_task_data = task_data<CreateTextureTaskData>(task);
            
            texture_task_data->load_data = data;
            texture_task_data->texture = &texture;
            
            task._function.Bind<&create_texture_task>();
            Context& context = Global::get_context();
            
            context._rendering_thread.enqueue_upload_task(task);
            context._load_wakeup_sema.wait();
            
            return texture;
        }
    }
}
