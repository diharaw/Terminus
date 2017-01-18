#include <Resource/texture_factory.h>
#include <Core/context.h>

namespace terminus
{
    namespace texture_factory
    {
        Texture* create(asset_common::ImageLoadData* data)
        {
            Task task;
            
            CreateTextureTaskData* task_data = task_data<CreateTextureTaskData>(task);
            
            Texture2D* texture = new Texture2D();
            
            task_data->load_data = data;
            
            task._function.Bind<&create_texture_task>();
            Global::get_context()._rendering_thread.enqueue_upload_task(task);
            
            return texture;
        }
        
        void create_texture_task(void* data)
        {
            CreateTextureTaskData* task_data = (CreateTextureTaskData*)data;
            context::get_render_device().CreateTexture2D(task_data->existing_texture,
                                                         task_data->load_data->width,
                                                         task_data->load_data->height,
                                                         task_data->load_data->bytes,
                                                         TextureFormat::R8G8B8A8_UNORM,
                                                         false);
            
            free(task_data->load_data->bytes);
            free(task_data->load_data);
        }
    }
}
